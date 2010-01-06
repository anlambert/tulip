#include <tulip/TulipPlugin.h>

using namespace std;
using namespace tlp;

namespace {
  const char * paramHelp[] = {
    // width
    HTML_HELP_OPEN() \
    HTML_HELP_DEF( "type", "int" ) \
    HTML_HELP_DEF( "default", "10" ) \
    HTML_HELP_BODY() \
    "This parameter defines the grid's node width." \
    HTML_HELP_CLOSE(),
    
    // height
    HTML_HELP_OPEN() \
    HTML_HELP_DEF( "type", "int" ) \
    HTML_HELP_DEF( "default", "10" ) \
    HTML_HELP_BODY() \
    "This parameter defines the grid's node height." \
    HTML_HELP_CLOSE(),
  };
}


struct Grid:public ImportModule {
  Grid(AlgorithmContext context):ImportModule(context) {
    addParameter<int>("width",paramHelp[0],"10");
    addParameter<int>("height",paramHelp[1],"10");
  }
  ~Grid(){}

  void buildRow(vector<node> &row, double height){
    LayoutProperty *layout = graph->getProperty<LayoutProperty>("viewLayout");
    int width=row.size();
    for (int i=0;i<width;++i) {
      row[i]=graph->addNode();
      layout->setNodeValue(row[i], Coord(i*2.0, height*2.0, 0));
    }
    for (int i=0;i<width-1;++i)
      graph->addEdge(row[i],row[i+1]);
  }
  
  void connectRow(vector<node> &row1,vector<node> &row2){
    assert(row1.size()==row2.size());
    int width=row1.size();
    for (int i=0;i<width;++i)
      graph->addEdge(row1[i],row2[i]);
  }

  bool import(const string &name) {
    int width = 10;
    int height = 10;
    if (dataSet!=0) {
      dataSet->get("width",width);
      dataSet->get("height",height);
    }
    vector<node> row[2];
    row[0].resize(width);
    row[1].resize(width);
    buildRow(row[0], height);
    int curRow=0;
    while(height>1) {
      int newRow=(curRow+1)%2;
      buildRow(row[newRow], height - 1);
      connectRow(row[curRow],row[newRow]);
      curRow=newRow;
      --height;
    }
    return true;
  }
};
IMPORTPLUGINOFGROUP(Grid,"Grid","Auber","02/12/2003","","1.0","Graphs")
