/**
 *
 * Copyright (C) 2019-2021  The Talipot developers
 *
 * Talipot is a fork of Tulip, created by David Auber
 * and the Tulip development Team from LaBRI, University of Bordeaux
 *
 * See the AUTHORS file at the top-level directory of this distribution
 * License: GNU General Public License version 3, or any later version
 * See top-level LICENSE file for more information
 *
 */

#if _MSC_VER
// disable Visual Studio warning about using this in base member initializer list
// this is not critical in our case as we only store a pointer
#pragma warning(disable : 4355)
#endif

#include <iostream>
#include <sstream>
#include <map>
#include <set>

#include <talipot/Observable.h>
#include <talipot/ConversionIterator.h>
#include <talipot/FilterIterator.h>
#include <talipot/ParallelTools.h>
#include <talipot/Exception.h>
#include <talipot/VectorGraph.h>

using namespace std;
using namespace tlp;

namespace tlp {

// we need a lock to protect the update
// of the observation graph
TLP_DEFINE_GLOBAL_LOCK(ObservableGraphUpdate);
//=================================
// _oPointer a pointer to the object represented by a node
static tlp::NodeProperty<Observable *> _oPointer;
//_oAlive whether an object has been deleted or not
static tlp::NodeProperty<bool> _oAlive;
//_oEventsToTreat the count of events scheduled to be treated by an object
// the object's associated node is deleted only when this count is null
// in order to prevent the node reuse and ensure the _oAlive associated value
static tlp::NodeProperty<unsigned int> _oEventsToTreat;
//_oType the type of relation between two Observable Objects
static tlp::EdgeProperty<unsigned char> _oType;
//=================================
class ObservationGraph : public VectorGraph {
public:
  static ObservationGraph _oGraph;
  static bool _oGraphDestroyed;

  ObservationGraph() {
    _oGraph.alloc(_oPointer);
    _oGraph.alloc(_oAlive);
    _oGraph.alloc(_oEventsToTreat);
    _oGraph.alloc(_oType);
  }

  ~ObservationGraph() {
    _oGraphDestroyed = true;
    _oGraph.free(_oPointer);
    _oGraph.free(_oAlive);
    _oGraph.free(_oEventsToTreat);
    _oGraph.free(_oType);
  }
};
// _oGraph the graph used to store all observers and connection between them.
ObservationGraph ObservationGraph::_oGraph;
bool ObservationGraph::_oGraphDestroyed = false;

//_oDelayedDelNode store deleted nodes, to remove them at the end of the notify
static std::vector<tlp::node> _oDelayedDelNode;
static std::set<std::pair<tlp::node, tlp::node>> _oDelayedEvents;
//_oNotifying counter of nested notify calls
static unsigned int _oNotifying = 0;
//_oUnholding counter of nested unhold calls
static unsigned int _oUnholding = 0;
//----------------------------------
unsigned int Observable::_oHoldCounter = 0;
bool Observable::_oDisabled = false;

class ObservableException : public tlp::Exception {
public:
  ObservableException(const std::string &desc) : tlp::Exception(desc) {}
};

//----------------------------------
Iterator<node> *Observable::getInObjects() const {
  assert(_n.isValid());
  return filterIterator(ObservationGraph::_oGraph.getInNodes(_n),
                        [&](node n) { return _oAlive[n]; });
}
//----------------------------------
Iterator<node> *Observable::getOutObjects() const {
  assert(_n.isValid());
  return filterIterator(ObservationGraph::_oGraph.getOutNodes(_n),
                        [&](node n) { return _oAlive[n]; });
}
//----------------------------------
node Observable::getNode() const {
  return _n;
}
//----------------------------------
node Observable::getBoundNode() {
  if (_n.isValid() == false) {
    _n = ObservationGraph::_oGraph.addNode();
    _oPointer[_n] = this;
    _oAlive[_n] = true;
    _oEventsToTreat[_n] = 0;
  }

  return _n;
}
//----------------------------------
unsigned int Observable::getSent() const {
  return sent;
}
//----------------------------------
unsigned int Observable::getReceived() const {
  return received;
}
//----------------------------------
bool Observable::isAlive(tlp::node n) {
  return _oAlive[n];
}
//----------------------------------
unsigned int Observable::getScheduled(tlp::node n) {
  return _oEventsToTreat[n];
}
//----------------------------------
Observable *Observable::getObject(node n) {
  assert(_oAlive[n]);

  if (!_oAlive[n]) {
    throw ObservableException("That object has been deleted it is no more accessible");
  }

  return _oPointer[n];
}
//----------------------------------
tlp::node Observable::getNode(const Observable *obs) {
  return obs->_n;
}
//----------------------------------
const tlp::VectorGraph &Observable::getObservableGraph() {
  return ObservationGraph::_oGraph;
}
//=================================
Event::Event(const Observable &sender, EventType type) : _sender(sender._n), _type(type) {
  assert(_type != TLP_DELETE);

  if (_type == TLP_DELETE) {
    throw ObservableException("It is forbidden to create a delete events, DeleteEvents are "
                              "autmotically generated at the observable destruction");
  }
}
//----------------------------------
Event::~Event() = default;
//----------------------------------
Observable *Event::sender() const {
  return Observable::getObject(_sender); /** only Observable can be use to create event */
}
//=================================
// define a class for an empty Iterator of Observable *
class NoObservableIterator : public Iterator<Observable *> {
public:
  Observable *next() override {
    return nullptr;
  }
  bool hasNext() override {
    return false;
  }
};
//=================================
void Observable::treatEvents(const std::vector<Event> &) {
#ifndef NDEBUG
  tlp::debug() << __PRETTY_FUNCTION__ << ": not implemented" << std::endl;
#endif
}
//=================================
void Observable::treatEvent(const Event &) {
#ifndef NDEBUG
  tlp::debug() << __PRETTY_FUNCTION__ << ": not implemented" << std::endl;
#endif
}
//=================================
Observable::Observable() : deleteMsgSent(false), queuedEvent(false), _n(node()) {
  sent = received = 0;
}
//----------------------------------
Observable::Observable(const Observable &) : deleteMsgSent(false), queuedEvent(false), _n(node()) {
  sent = received = 0;
}
//----------------------------------
Observable &Observable::operator=(const Observable &) {
#ifndef NDEBUG
  tlp::debug() << "[Observable Warning]: Observable object should reimplement their operator= else "
                  "nothing is copied"
               << endl;
#endif
  return *this;
}
//----------------------------------
Observable::~Observable() {
  if (ObservationGraph::_oGraphDestroyed || _n.isValid() == false) {
    return;
  }

  if (!deleteMsgSent) {
    observableDeleted();
  }

  TLP_GLOBALLY_LOCK_SECTION(ObservableGraphUpdate) {
    assert(_oAlive[_n]);

    if (!_oAlive[_n]) {
      tlp::error() << "[ERROR]: in " << __PRETTY_FUNCTION__ << " at " << __FILE__ << ':' << __LINE__
                   << endl;
      tlp::error() << "Observable object has already been deleted, possible double free!!!" << endl;
      std::terminate();
    }

    _oAlive[_n] = false;

    bool noDelay = (_oNotifying == 0) && (_oUnholding == 0) && (_oHoldCounter == 0);

    if ((noDelay == false) && (_oEventsToTreat[_n] == 0)) {
      // _n cannot be deleted only if it is observed
      // then its deletion is delayed until the observers are unhold
      noDelay = true;
      for (auto e : ObservationGraph::_oGraph.star(_n)) {
        if (_n == ObservationGraph::_oGraph.target(e) && _oType[e] & OBSERVER) {
          noDelay = false;
          break;
        }
      }
    }

    if (noDelay) {
      assert(_oEventsToTreat[_n] == 0);
      ObservationGraph::_oGraph.delNode(_n);
    } else {
      _oDelayedDelNode.push_back(_n);
      ObservationGraph::_oGraph.delEdges(_n);
    }
  }
  TLP_GLOBALLY_UNLOCK_SECTION(ObservableGraphUpdate);
}

//----------------------------------------
void Observable::holdObservers() {
  TLP_GLOBALLY_LOCK_SECTION(ObservableGraphUpdate) {
    ++_oHoldCounter;
  }
  TLP_GLOBALLY_UNLOCK_SECTION(ObservableGraphUpdate);
}
//----------------------------------------
void Observable::unholdObservers() {
  {
    assert(_oHoldCounter > 0);

    if (_oHoldCounter == 0) {
#ifndef NDEBUG
      throw ObservableException("unhold call without a previous call to hold");
#endif
      return;
    }

    --_oHoldCounter;
    {
      if (_oHoldCounter > 0 || _oDelayedEvents.empty()) {
        return;
      }

      ++_oUnholding;
      ++_oHoldCounter; /** rehold the observer to buffer message sent during unholding */

      set<pair<node, node>> backupEvents;
      backupEvents.swap(_oDelayedEvents);
      map<node, vector<Event>> preparedEvents;

      for (const auto &[src, tgt] : backupEvents) {
        if (_oAlive[src]) {
          auto *sender = static_cast<Observable *>(_oPointer[src]);
          sender->queuedEvent = false;
          if (_oAlive[tgt]) {
            _oEventsToTreat[tgt] += 1;
            preparedEvents[tgt].push_back(Event(*sender, Event::TLP_MODIFICATION));
          }
        }
      }

      {

        for (const auto &[n, events] : preparedEvents) {
          // treat scheduled events
          _oEventsToTreat[n] -= events.size();

          if (_oAlive[n]) {
            auto *obs = static_cast<Observable *>(_oPointer[n]);
            ++(obs->received);
            obs->treatEvents(events);
          }
        }
      }

      --_oUnholding;
      unholdObservers(); /** treat queued events during the unhold */
      updateObserverGraph();

      if (_oHoldCounter > 0) {
        throw ObservableException("Observable after unholdd call, bad nested hold/unhold function "
                                  "call in an Observer:: update() function");
      }
    }
  }
}
//----------------------------------------
Iterator<Observable *> *Observable::getOnlookers() const {
  if (isBound()) {
    assert(_oAlive[_n]);

    if (!_oAlive[_n]) {
      throw ObservableException("getObservers called on a deleted Observable");
    }

    return conversionIterator<Observable *>(getInObjects(),
                                            [&](const node &n) { return _oPointer[n]; });
  }

  return new NoObservableIterator();
}
//----------------------------------------
void Observable::addOnlooker(const Observable &obs, OBSERVABLEEDGETYPE type) const {
  TLP_GLOBALLY_LOCK_SECTION(ObservableGraphUpdate) {
    assert(!isBound() || _oAlive[_n]);

    if (isBound() && !_oAlive[_n]) {
      throw ObservableException("addObserver called on a deleted Observable");
    }

    // check for an existing link
    edge link;

    if (isBound() && obs.isBound()) {
      link = ObservationGraph::_oGraph.existEdge(obs._n, _n);
    }

    if (!link.isValid()) {
      // add new link
      // at this time both Observables need to be bound
      link = ObservationGraph::_oGraph.addEdge(const_cast<Observable &>(obs).getBoundNode(),
                                               const_cast<Observable *>(this)->getBoundNode());
      _oType[link] = type;
    } else {
      // add the bit for the given type on the edge
      _oType[link] |= type;
    }
  }
  TLP_GLOBALLY_UNLOCK_SECTION(ObservableGraphUpdate);
}
//----------------------------------------
void Observable::addObserver(Observable *const observer) const {
  assert(observer != nullptr);
  queuedEvent = false;
  addOnlooker(*observer, OBSERVER);
}
//----------------------------------------
void Observable::addListener(Observable *const listener) const {
  assert(listener != nullptr);
  addOnlooker(*listener, LISTENER);
}
//----------------------------------------
void Observable::observableDeleted() {
  assert(deleteMsgSent == false);

  if (deleteMsgSent) {
    throw ObservableException("Delete message has been sent several time.");
  }

  deleteMsgSent = true;

  if (hasOnlookers()) {
    Event msg(*this, Event::TLP_INVALID); // create a modify event to prevent raise exception,
                                          // (hack) to forbid creation of Delete exception without
                                          // calling that function
    msg._type = Event::TLP_DELETE;
    sendEvent(msg);
  }
}
//----------------------------------------
void Observable::sendEvent(const Event &message) {
  if ((_oDisabled && message._type != Event::TLP_DELETE) || !isBound()) {
    return;
  }

  // cerr << "send event " << _oPointer[_n] << " " << message.type() << " indeg " <<
  // ObservationGraph::_oGraph.indeg(_n) << " outdeg: " << ObservationGraph::_oGraph.outdeg(_n) <<
  // endl;
  if (!ObservationGraph::_oGraph.isElement(_n) || !_oAlive[_n]) {
    throw ObservableException("Notify called on a deleted Observable");
  }

  const unsigned int RECCALL = 200;

  if (_oNotifying > RECCALL) {
    std::stringstream str;
    str << "Maximum number of nested calls (" << RECCALL << ") reached";
    throw ObservableException(str.str());
  }

  node backn = _n; /** to keep trace of the node if the observable is deleted during the
                      notification, in that crazy case, (*this) is dead thus n is not accessible*/
  ++_oNotifying;

  // create two separate list of observer & listeners
  // could be extended if we need recorders
  vector<pair<Observable *, node>> observerTonotify;
  vector<pair<Observable *, node>> listenerTonotify;
  bool delayedEventAdded = false;
  for (auto e : ObservationGraph::_oGraph.star(_n)) {
    node src = ObservationGraph::_oGraph.source(e);

    if (_n != src && _oAlive[src]) {
      Observable *obs = _oPointer[src];
      assert(obs != nullptr);

      if ((_oType[e] & OBSERVER) && (message.type() != Event::TLP_INFORMATION)) {
        if (_oHoldCounter == 0 || message.type() == Event::TLP_DELETE) {
          // schedule event
          _oEventsToTreat[backn] += 1;
          _oEventsToTreat[src] += 1;
          observerTonotify.push_back({obs, src});
        } else if (!queuedEvent) {
          delayedEventAdded = true;
          TLP_GLOBALLY_LOCK_SECTION(ObservableGraphUpdate) {
            _oDelayedEvents.insert({_n, src});
          }
          TLP_GLOBALLY_UNLOCK_SECTION(ObservableGraphUpdate);
        }
      }

      if (_oType[e] & LISTENER) {
        // schedule event
        _oEventsToTreat[backn] += 1;
        _oEventsToTreat[src] += 1;
        listenerTonotify.push_back({obs, src});
      }
    }
  }

  if (delayedEventAdded) {
    queuedEvent = true;
  }

  // send message to listeners
  for (const auto &[obs, n] : listenerTonotify) {
    if (n == backn && message.type() == Event::TLP_DELETE) {
      tlp::debug() << "[Observable info]: An observable onlook itself Event::DELETE msg can't be "
                      "sent to it."
                   << endl;
      // treat scheduled event
      _oEventsToTreat[backn] -= 2;
      continue;
    }

    // treat scheduled event
    _oEventsToTreat[n] -= 1;

    if (_oAlive[n]) { // other listeners/observers could be destroyed during the treat
                      // event
      ++(obs->received);
      obs->treatEvent(message);
    }

    // we decrement after treating event
    // to prevent reuse of backn
    _oEventsToTreat[backn] -= 1;

    assert(_oAlive[backn]);

    if (!_oAlive[backn]) {
      throw ObservableException("An observable has been deleted during the notifification of its "
                                "observer (ie. an observer has deleted its caller during an "
                                "update)");
    }
  }

  // send simple event to observers
  if (!observerTonotify.empty()) {
    vector<Event> tmp(1, message);

    for (const auto &[obs, n] : observerTonotify) {
      if (n == backn && message.type() == Event::TLP_DELETE) {
        tlp::debug() << "[Observable info]: An observable onlook itself Event::DELETE msg can't be "
                        "sent to it."
                     << endl;
        // treat scheduled event
        _oEventsToTreat[backn] -= 2;
        continue;
      }

      // treat scheduled event
      _oEventsToTreat[n] -= 1;

      if (_oAlive[n]) { // other listeners/observers could be destroyed during the treat
                        // event
        ++(obs->received);
        obs->treatEvents(tmp);
      }

      // we decrement after treating event
      // to prevent reuse of backn
      _oEventsToTreat[backn] -= 1;

      assert(_oAlive[backn]);

      if (!_oAlive[backn]) {
        throw ObservableException("An observable has been deleted during the notifification of its "
                                  "observer (ie. an observer has deleted its caller during an "
                                  "update)");
      }
    }
  }

  ++sent;
  --_oNotifying;

  if (!observerTonotify.empty() || !listenerTonotify.empty() ||
      message.type() == Event::TLP_DELETE) {
    updateObserverGraph();
  }
}
//----------------------------------------
void Observable::updateObserverGraph() {
  if (_oNotifying == 0 && _oUnholding == 0 && _oHoldCounter == 0) {
    TLP_GLOBALLY_LOCK_SECTION(ObservableGraphUpdate) {
      for (auto toDel : _oDelayedDelNode) {
        if (_oEventsToTreat[toDel] == 0) {
          ObservationGraph::_oGraph.delNode(toDel);
        }
      }
    }
    TLP_GLOBALLY_UNLOCK_SECTION(ObservableGraphUpdate);
    _oDelayedDelNode.clear();
  }
}
//----------------------------------------
void Observable::removeOnlooker(const Observable &obs, OBSERVABLEEDGETYPE type) const {
  // nothing to do if one of the observables is unbound
  if (!isBound() || !obs.isBound()) {
    return;
  }

  TLP_GLOBALLY_LOCK_SECTION(ObservableGraphUpdate) {
    assert(_oAlive[_n]);

    if (!_oAlive[_n]) {
      throw ObservableException("removeOnlooker called on a deleted Observable");
    }

    edge link(ObservationGraph::_oGraph.existEdge(obs._n, _n));

    if (link.isValid()) {
      _oType[link] = _oType[link] &
                     ~type; // bitwise operation to remove the bit  for the given type on the edge

      if (_oType[link] == 0) {
        ObservationGraph::_oGraph.delEdge(link);
      }
    }
  }
  TLP_GLOBALLY_UNLOCK_SECTION(ObservableGraphUpdate);
}
//----------------------------------------
void Observable::removeObserver(Observable *const observerver) const {
  assert(observerver != nullptr);
  removeOnlooker(*observerver, OBSERVER);
}
//----------------------------------------
void Observable::removeListener(Observable *const listener) const {
  assert(listener != nullptr);
  removeOnlooker(*listener, LISTENER);
}
//----------------------------------------
bool Observable::hasOnlookers() const {
  if (!isBound()) {
    return false;
  }

  assert(_oAlive[_n]);

  if (!_oAlive[_n]) {
    throw ObservableException("hasOnlookers called on a deleted Observable");
  }

  return ObservationGraph::_oGraph.indeg(_n) > 0;
}
//----------------------------------------
unsigned int Observable::countListeners() const {
  if (!hasOnlookers()) {
    return 0;
  }

  unsigned int count = 0;
  for (auto e : ObservationGraph::_oGraph.star(_n)) {
    if (_n == ObservationGraph::_oGraph.target(e) && (_oType[e] & LISTENER)) {
      ++count;
    }
  }
  return count;
}
//----------------------------------------
unsigned int Observable::countObservers() const {
  if (!hasOnlookers()) {
    return 0;
  }

  unsigned int count = 0;
  for (auto e : ObservationGraph::_oGraph.star(_n)) {
    if (_n == ObservationGraph::_oGraph.target(e) && (_oType[e] & OBSERVER)) {
      ++count;
    }
  }
  return count;
}
}
