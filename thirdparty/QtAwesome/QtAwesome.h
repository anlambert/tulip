/**
 * QtAwesome - use font-awesome (or other font icons) in your c++ / Qt Application
 *
 * MIT Licensed
 *
 * Copyright 2013-2015 - Reliable Bits Software by Blommers IT. All Rights Reserved.
 * Author Rick Blommers
 */

#ifndef QTAWESOME_H
#define QTAWESOME_H

#include <QIcon>
#include <QIconEngine>
#include <QPainter>
#include <QRect>
#include <QVariantMap>

#undef linux

/// A list of all icon-names with the codepoint (unicode-value) on the right
/// You can use the names on the page  http://fortawesome.github.io/Font-Awesome/design.html
class fa {

public:

  enum iconCodePoint {

    fa_500px             = 0xf26e,
    adjust               = 0xf042,
    adn                  = 0xf170,
    aligncenter          = 0xf037,
    alignjustify         = 0xf039,
    alignleft            = 0xf036,
    alignright           = 0xf038,
    amazon               = 0xf270,
    ambulance            = 0xf0f9,
    anchor               = 0xf13d,
    android              = 0xf17b,
    angellist            = 0xf209,
    angledoubledown      = 0xf103,
    angledoubleleft      = 0xf100,
    angledoubleright     = 0xf101,
    angledoubleup        = 0xf102,
    angledown            = 0xf107,
    angleleft            = 0xf104,
    angleright           = 0xf105,
    angleup              = 0xf106,
    apple                = 0xf179,
    archive              = 0xf187,
    areachart            = 0xf1fe,
    arrowcircledown      = 0xf0ab,
    arrowcircleleft      = 0xf0a8,
    arrowcircleodown     = 0xf01a,
    arrowcircleoleft     = 0xf190,
    arrowcircleoright    = 0xf18e,
    arrowcircleoup       = 0xf01b,
    arrowcircleright     = 0xf0a9,
    arrowcircleup        = 0xf0aa,
    arrowdown            = 0xf063,
    arrowleft            = 0xf060,
    arrowright           = 0xf061,
    arrowup              = 0xf062,
    arrows               = 0xf047,
    arrowsalt            = 0xf0b2,
    arrowsh              = 0xf07e,
    arrowsv              = 0xf07d,
    asterisk             = 0xf069,
    at                   = 0xf1fa,
    automobile           = 0xf1b9,
    backward             = 0xf04a,
    balancescale         = 0xf24e,
    ban                  = 0xf05e,
    bank                 = 0xf19c,
    barchart             = 0xf080,
    barcharto            = 0xf080,
    barcode              = 0xf02a,
    bars                 = 0xf0c9,
    battery0             = 0xf244,
    battery1             = 0xf243,
    battery2             = 0xf242,
    battery3             = 0xf241,
    battery4             = 0xf240,
    batteryempty         = 0xf244,
    batteryfull          = 0xf240,
    batteryhalf          = 0xf242,
    batteryquarter       = 0xf243,
    batterythreequarters = 0xf241,
    bed                  = 0xf236,
    beer                 = 0xf0fc,
    behance              = 0xf1b4,
    behancesquare        = 0xf1b5,
    bell                 = 0xf0f3,
    bello                = 0xf0a2,
    bellslash            = 0xf1f6,
    bellslasho           = 0xf1f7,
    bicycle              = 0xf206,
    binoculars           = 0xf1e5,
    birthdaycake         = 0xf1fd,
    bitbucket            = 0xf171,
    bitbucketsquare      = 0xf172,
    bitcoin              = 0xf15a,
    blacktie             = 0xf27e,
    bold                 = 0xf032,
    bolt                 = 0xf0e7,
    bomb                 = 0xf1e2,
    book                 = 0xf02d,
    bookmark             = 0xf02e,
    bookmarko            = 0xf097,
    briefcase            = 0xf0b1,
    btc                  = 0xf15a,
    bug                  = 0xf188,
    building             = 0xf1ad,
    buildingo            = 0xf0f7,
    bullhorn             = 0xf0a1,
    bullseye             = 0xf140,
    bus                  = 0xf207,
    buysellads           = 0xf20d,
    cab                  = 0xf1ba,
    calculator           = 0xf1ec,
    calendar             = 0xf073,
    calendarchecko       = 0xf274,
    calendarminuso       = 0xf272,
    calendaro            = 0xf133,
    calendarpluso        = 0xf271,
    calendartimeso       = 0xf273,
    camera               = 0xf030,
    cameraretro          = 0xf083,
    car                  = 0xf1b9,
    caretdown            = 0xf0d7,
    caretleft            = 0xf0d9,
    caretright           = 0xf0da,
    caretsquareodown     = 0xf150,
    caretsquareoleft     = 0xf191,
    caretsquareoright    = 0xf152,
    caretsquareoup       = 0xf151,
    caretup              = 0xf0d8,
    cartarrowdown        = 0xf218,
    cartplus             = 0xf217,
    cc                   = 0xf20a,
    ccamex               = 0xf1f3,
    ccdinersclub         = 0xf24c,
    ccdiscover           = 0xf1f2,
    ccjcb                = 0xf24b,
    ccmastercard         = 0xf1f1,
    ccpaypal             = 0xf1f4,
    ccstripe             = 0xf1f5,
    ccvisa               = 0xf1f0,
    certificate          = 0xf0a3,
    chain                = 0xf0c1,
    chainbroken          = 0xf127,
    check                = 0xf00c,
    checkcircle          = 0xf058,
    checkcircleo         = 0xf05d,
    checksquare          = 0xf14a,
    checksquareo         = 0xf046,
    chevroncircledown    = 0xf13a,
    chevroncircleleft    = 0xf137,
    chevroncircleright   = 0xf138,
    chevroncircleup      = 0xf139,
    chevrondown          = 0xf078,
    chevronleft          = 0xf053,
    chevronright         = 0xf054,
    chevronup            = 0xf077,
    child                = 0xf1ae,
    chrome               = 0xf268,
    circle               = 0xf111,
    circleo              = 0xf10c,
    circleonotch         = 0xf1ce,
    circlethin           = 0xf1db,
    clipboard            = 0xf0ea,
    clocko               = 0xf017,
    clone                = 0xf24d,
    close                = 0xf00d,
    cloud                = 0xf0c2,
    clouddownload        = 0xf0ed,
    cloudupload          = 0xf0ee,
    cny                  = 0xf157,
    code                 = 0xf121,
    codefork             = 0xf126,
    codepen              = 0xf1cb,
    coffee               = 0xf0f4,
    cog                  = 0xf013,
    cogs                 = 0xf085,
    columns              = 0xf0db,
    comment              = 0xf075,
    commento             = 0xf0e5,
    commenting           = 0xf27a,
    commentingo          = 0xf27b,
    comments             = 0xf086,
    commentso            = 0xf0e6,
    compass              = 0xf14e,
    compress             = 0xf066,
    connectdevelop       = 0xf20e,
    contao               = 0xf26d,
    copy                 = 0xf0c5,
    copyright            = 0xf1f9,
    creativecommons      = 0xf25e,
    creditcard           = 0xf09d,
    crop                 = 0xf125,
    crosshairs           = 0xf05b,
    css3                 = 0xf13c,
    cube                 = 0xf1b2,
    cubes                = 0xf1b3,
    cut                  = 0xf0c4,
    cutlery              = 0xf0f5,
    dashboard            = 0xf0e4,
    dashcube             = 0xf210,
    database             = 0xf1c0,
    dedent               = 0xf03b,
    delicious            = 0xf1a5,
    desktop              = 0xf108,
    deviantart           = 0xf1bd,
    diamond              = 0xf219,
    digg                 = 0xf1a6,
    dollar               = 0xf155,
    dotcircleo           = 0xf192,
    download             = 0xf019,
    dribbble             = 0xf17d,
    dropbox              = 0xf16b,
    drupal               = 0xf1a9,
    edit                 = 0xf044,
    eject                = 0xf052,
    ellipsish            = 0xf141,
    ellipsisv            = 0xf142,
    empire               = 0xf1d1,
    envelope             = 0xf0e0,
    envelopeo            = 0xf003,
    envelopesquare       = 0xf199,
    eraser               = 0xf12d,
    eur                  = 0xf153,
    euro                 = 0xf153,
    exchange             = 0xf0ec,
    exclamation          = 0xf12a,
    exclamationcircle    = 0xf06a,
    exclamationtriangle  = 0xf071,
    expand               = 0xf065,
    expeditedssl         = 0xf23e,
    externallink         = 0xf08e,
    externallinksquare   = 0xf14c,
    eye                  = 0xf06e,
    eyeslash             = 0xf070,
    eyedropper           = 0xf1fb,
    facebook             = 0xf09a,
    facebookf            = 0xf09a,
    facebookofficial     = 0xf230,
    facebooksquare       = 0xf082,
    fastbackward         = 0xf049,
    fastforward          = 0xf050,
    fax                  = 0xf1ac,
    feed                 = 0xf09e,
    female               = 0xf182,
    fighterjet           = 0xf0fb,
    file                 = 0xf15b,
    filearchiveo         = 0xf1c6,
    fileaudioo           = 0xf1c7,
    filecodeo            = 0xf1c9,
    fileexcelo           = 0xf1c3,
    fileimageo           = 0xf1c5,
    filemovieo           = 0xf1c8,
    fileo                = 0xf016,
    filepdfo             = 0xf1c1,
    filephotoo           = 0xf1c5,
    filepictureo         = 0xf1c5,
    filepowerpointo      = 0xf1c4,
    filesoundo           = 0xf1c7,
    filetext             = 0xf15c,
    filetexto            = 0xf0f6,
    filevideoo           = 0xf1c8,
    filewordo            = 0xf1c2,
    filezipo             = 0xf1c6,
    fileso               = 0xf0c5,
    film                 = 0xf008,
    filter               = 0xf0b0,
    fire                 = 0xf06d,
    fireextinguisher     = 0xf134,
    firefox              = 0xf269,
    flag                 = 0xf024,
    flagcheckered        = 0xf11e,
    flago                = 0xf11d,
    flash                = 0xf0e7,
    flask                = 0xf0c3,
    flickr               = 0xf16e,
    floppyo              = 0xf0c7,
    folder               = 0xf07b,
    foldero              = 0xf114,
    folderopen           = 0xf07c,
    folderopeno          = 0xf115,
    font                 = 0xf031,
    fonticons            = 0xf280,
    forumbee             = 0xf211,
    forward              = 0xf04e,
    foursquare           = 0xf180,
    frowno               = 0xf119,
    futbolo              = 0xf1e3,
    gamepad              = 0xf11b,
    gavel                = 0xf0e3,
    gbp                  = 0xf154,
    ge                   = 0xf1d1,
    gear                 = 0xf013,
    gears                = 0xf085,
    genderless           = 0xf22d,
    getpocket            = 0xf265,
    gg                   = 0xf260,
    ggcircle             = 0xf261,
    gift                 = 0xf06b,
    git                  = 0xf1d3,
    gitsquare            = 0xf1d2,
    github               = 0xf09b,
    githubalt            = 0xf113,
    githubsquare         = 0xf092,
    gittip               = 0xf184,
    glass                = 0xf000,
    globe                = 0xf0ac,
    google               = 0xf1a0,
    googleplus           = 0xf0d5,
    googleplussquare     = 0xf0d4,
    googlewallet         = 0xf1ee,
    graduationcap        = 0xf19d,
    gratipay             = 0xf184,
    group                = 0xf0c0,
    hsquare              = 0xf0fd,
    hackernews           = 0xf1d4,
    handgrabo            = 0xf255,
    handlizardo          = 0xf258,
    handodown            = 0xf0a7,
    handoleft            = 0xf0a5,
    handoright           = 0xf0a4,
    handoup              = 0xf0a6,
    handpapero           = 0xf256,
    handpeaceo           = 0xf25b,
    handpointero         = 0xf25a,
    handrocko            = 0xf255,
    handscissorso        = 0xf257,
    handspocko           = 0xf259,
    handstopo            = 0xf256,
    hddo                 = 0xf0a0,
    header               = 0xf1dc,
    headphones           = 0xf025,
    heart                = 0xf004,
    hearto               = 0xf08a,
    heartbeat            = 0xf21e,
    history              = 0xf1da,
    home                 = 0xf015,
    hospitalo            = 0xf0f8,
    hotel                = 0xf236,
    hourglass            = 0xf254,
    hourglass1           = 0xf251,
    hourglass2           = 0xf252,
    hourglass3           = 0xf253,
    hourglassend         = 0xf253,
    hourglasshalf        = 0xf252,
    hourglasso           = 0xf250,
    hourglassstart       = 0xf251,
    houzz                = 0xf27c,
    html5                = 0xf13b,
    icursor              = 0xf246,
    ils                  = 0xf20b,
    image                = 0xf03e,
    inbox                = 0xf01c,
    indent               = 0xf03c,
    industry             = 0xf275,
    info                 = 0xf129,
    infocircle           = 0xf05a,
    inr                  = 0xf156,
    instagram            = 0xf16d,
    institution          = 0xf19c,
    internetexplorer     = 0xf26b,
    intersex             = 0xf224,
    ioxhost              = 0xf208,
    italic               = 0xf033,
    joomla               = 0xf1aa,
    jpy                  = 0xf157,
    jsfiddle             = 0xf1cc,
    key                  = 0xf084,
    keyboardo            = 0xf11c,
    krw                  = 0xf159,
    language             = 0xf1ab,
    laptop               = 0xf109,
    lastfm               = 0xf202,
    lastfmsquare         = 0xf203,
    leaf                 = 0xf06c,
    leanpub              = 0xf212,
    legal                = 0xf0e3,
    lemono               = 0xf094,
    leveldown            = 0xf149,
    levelup              = 0xf148,
    lifebouy             = 0xf1cd,
    lifebuoy             = 0xf1cd,
    lifering             = 0xf1cd,
    lifesaver            = 0xf1cd,
    lightbulbo           = 0xf0eb,
    linechart            = 0xf201,
    link                 = 0xf0c1,
    linkedin             = 0xf0e1,
    linkedinsquare       = 0xf08c,
    linux                = 0xf17c,
    list                 = 0xf03a,
    listalt              = 0xf022,
    listol               = 0xf0cb,
    listul               = 0xf0ca,
    locationarrow        = 0xf124,
    lock                 = 0xf023,
    longarrowdown        = 0xf175,
    longarrowleft        = 0xf177,
    longarrowright       = 0xf178,
    longarrowup          = 0xf176,
    magic                = 0xf0d0,
    magnet               = 0xf076,
    mailforward          = 0xf064,
    mailreply            = 0xf112,
    mailreplyall         = 0xf122,
    male                 = 0xf183,
    map                  = 0xf279,
    mapmarker            = 0xf041,
    mapo                 = 0xf278,
    mappin               = 0xf276,
    mapsigns             = 0xf277,
    mars                 = 0xf222,
    marsdouble           = 0xf227,
    marsstroke           = 0xf229,
    marsstrokeh          = 0xf22b,
    marsstrokev          = 0xf22a,
    maxcdn               = 0xf136,
    meanpath             = 0xf20c,
    medium               = 0xf23a,
    medkit               = 0xf0fa,
    meho                 = 0xf11a,
    mercury              = 0xf223,
    microphone           = 0xf130,
    microphoneslash      = 0xf131,
    minus                = 0xf068,
    minuscircle          = 0xf056,
    minussquare          = 0xf146,
    minussquareo         = 0xf147,
    mobile               = 0xf10b,
    mobilephone          = 0xf10b,
    money                = 0xf0d6,
    moono                = 0xf186,
    mortarboard          = 0xf19d,
    motorcycle           = 0xf21c,
    mousepointer         = 0xf245,
    music                = 0xf001,
    navicon              = 0xf0c9,
    neuter               = 0xf22c,
    newspapero           = 0xf1ea,
    objectgroup          = 0xf247,
    objectungroup        = 0xf248,
    odnoklassniki        = 0xf263,
    odnoklassnikisquare  = 0xf264,
    opencart             = 0xf23d,
    openid               = 0xf19b,
    opera                = 0xf26a,
    optinmonster         = 0xf23c,
    outdent              = 0xf03b,
    pagelines            = 0xf18c,
    paintbrush           = 0xf1fc,
    paperplane           = 0xf1d8,
    paperplaneo          = 0xf1d9,
    paperclip            = 0xf0c6,
    paragraph            = 0xf1dd,
    paste                = 0xf0ea,
    pause                = 0xf04c,
    paw                  = 0xf1b0,
    paypal               = 0xf1ed,
    pencil               = 0xf040,
    pencilsquare         = 0xf14b,
    pencilsquareo        = 0xf044,
    phone                = 0xf095,
    phonesquare          = 0xf098,
    photo                = 0xf03e,
    pictureo             = 0xf03e,
    piechart             = 0xf200,
    piedpiper            = 0xf1a7,
    piedpiperalt         = 0xf1a8,
    pinterest            = 0xf0d2,
    pinterestp           = 0xf231,
    pinterestsquare      = 0xf0d3,
    plane                = 0xf072,
    play                 = 0xf04b,
    playcircle           = 0xf144,
    playcircleo          = 0xf01d,
    plug                 = 0xf1e6,
    plus                 = 0xf067,
    pluscircle           = 0xf055,
    plussquare           = 0xf0fe,
    plussquareo          = 0xf196,
    poweroff             = 0xf011,
    print                = 0xf02f,
    puzzlepiece          = 0xf12e,
    qq                   = 0xf1d6,
    qrcode               = 0xf029,
    question             = 0xf128,
    questioncircle       = 0xf059,
    quoteleft            = 0xf10d,
    quoteright           = 0xf10e,
    ra                   = 0xf1d0,
    random               = 0xf074,
    rebel                = 0xf1d0,
    recycle              = 0xf1b8,
    reddit               = 0xf1a1,
    redditsquare         = 0xf1a2,
    refresh              = 0xf021,
    registered           = 0xf25d,
    remove               = 0xf00d,
    renren               = 0xf18b,
    reorder              = 0xf0c9,
    repeat               = 0xf01e,
    reply                = 0xf112,
    replyall             = 0xf122,
    retweet              = 0xf079,
    rmb                  = 0xf157,
    road                 = 0xf018,
    rocket               = 0xf135,
    rotateleft           = 0xf0e2,
    rotateright          = 0xf01e,
    rouble               = 0xf158,
    rss                  = 0xf09e,
    rsssquare            = 0xf143,
    rub                  = 0xf158,
    ruble                = 0xf158,
    rupee                = 0xf156,
    safari               = 0xf267,
    save                 = 0xf0c7,
    scissors             = 0xf0c4,
    search               = 0xf002,
    searchminus          = 0xf010,
    searchplus           = 0xf00e,
    sellsy               = 0xf213,
    send                 = 0xf1d8,
    sendo                = 0xf1d9,
    server               = 0xf233,
    share                = 0xf064,
    sharealt             = 0xf1e0,
    sharealtsquare       = 0xf1e1,
    sharesquare          = 0xf14d,
    sharesquareo         = 0xf045,
    shekel               = 0xf20b,
    sheqel               = 0xf20b,
    shield               = 0xf132,
    ship                 = 0xf21a,
    shirtsinbulk         = 0xf214,
    shoppingcart         = 0xf07a,
    signin               = 0xf090,
    signout              = 0xf08b,
    signal               = 0xf012,
    simplybuilt          = 0xf215,
    sitemap              = 0xf0e8,
    skyatlas             = 0xf216,
    skype                = 0xf17e,
    slack                = 0xf198,
    sliders              = 0xf1de,
    slideshare           = 0xf1e7,
    smileo               = 0xf118,
    soccerballo          = 0xf1e3,
    sort                 = 0xf0dc,
    sortalphaasc         = 0xf15d,
    sortalphadesc        = 0xf15e,
    sortamountasc        = 0xf160,
    sortamountdesc       = 0xf161,
    sortasc              = 0xf0de,
    sortdesc             = 0xf0dd,
    sortdown             = 0xf0dd,
    sortnumericasc       = 0xf162,
    sortnumericdesc      = 0xf163,
    sortup               = 0xf0de,
    soundcloud           = 0xf1be,
    spaceshuttle         = 0xf197,
    spinner              = 0xf110,
    spoon                = 0xf1b1,
    spotify              = 0xf1bc,
    square               = 0xf0c8,
    squareo              = 0xf096,
    stackexchange        = 0xf18d,
    stackoverflow        = 0xf16c,
    star                 = 0xf005,
    starhalf             = 0xf089,
    starhalfempty        = 0xf123,
    starhalffull         = 0xf123,
    starhalfo            = 0xf123,
    staro                = 0xf006,
    steam                = 0xf1b6,
    steamsquare          = 0xf1b7,
    stepbackward         = 0xf048,
    stepforward          = 0xf051,
    stethoscope          = 0xf0f1,
    stickynote           = 0xf249,
    stickynoteo          = 0xf24a,
    stop                 = 0xf04d,
    streetview           = 0xf21d,
    strikethrough        = 0xf0cc,
    stumbleupon          = 0xf1a4,
    stumbleuponcircle    = 0xf1a3,
    subscript            = 0xf12c,
    subway               = 0xf239,
    suitcase             = 0xf0f2,
    suno                 = 0xf185,
    superscript          = 0xf12b,
    support              = 0xf1cd,
    table                = 0xf0ce,
    tablet               = 0xf10a,
    tachometer           = 0xf0e4,
    tag                  = 0xf02b,
    tags                 = 0xf02c,
    tasks                = 0xf0ae,
    taxi                 = 0xf1ba,
    television           = 0xf26c,
    tencentweibo         = 0xf1d5,
    terminal             = 0xf120,
    textheight           = 0xf034,
    textwidth            = 0xf035,
    th                   = 0xf00a,
    thlarge              = 0xf009,
    thlist               = 0xf00b,
    thumbtack            = 0xf08d,
    thumbsdown           = 0xf165,
    thumbsodown          = 0xf088,
    thumbsoup            = 0xf087,
    thumbsup             = 0xf164,
    ticket               = 0xf145,
    times                = 0xf00d,
    timescircle          = 0xf057,
    timescircleo         = 0xf05c,
    tint                 = 0xf043,
    toggledown           = 0xf150,
    toggleleft           = 0xf191,
    toggleoff            = 0xf204,
    toggleon             = 0xf205,
    toggleright          = 0xf152,
    toggleup             = 0xf151,
    trademark            = 0xf25c,
    train                = 0xf238,
    transgender          = 0xf224,
    transgenderalt       = 0xf225,
    trash                = 0xf1f8,
    trasho               = 0xf014,
    tree                 = 0xf1bb,
    trello               = 0xf181,
    tripadvisor          = 0xf262,
    trophy               = 0xf091,
    truck                = 0xf0d1,
    fa_try               = 0xf195,
    tty                  = 0xf1e4,
    tumblr               = 0xf173,
    tumblrsquare         = 0xf174,
    turkishlira          = 0xf195,
    tv                   = 0xf26c,
    twitch               = 0xf1e8,
    twitter              = 0xf099,
    twittersquare        = 0xf081,
    umbrella             = 0xf0e9,
    underline            = 0xf0cd,
    undo                 = 0xf0e2,
    university           = 0xf19c,
    unlink               = 0xf127,
    unlock               = 0xf09c,
    unlockalt            = 0xf13e,
    unsorted             = 0xf0dc,
    upload               = 0xf093,
    usd                  = 0xf155,
    user                 = 0xf007,
    usermd               = 0xf0f0,
    userplus             = 0xf234,
    usersecret           = 0xf21b,
    usertimes            = 0xf235,
    users                = 0xf0c0,
    venus                = 0xf221,
    venusdouble          = 0xf226,
    venusmars            = 0xf228,
    viacoin              = 0xf237,
    videocamera          = 0xf03d,
    vimeo                = 0xf27d,
    vimeosquare          = 0xf194,
    vine                 = 0xf1ca,
    vk                   = 0xf189,
    volumedown           = 0xf027,
    volumeoff            = 0xf026,
    volumeup             = 0xf028,
    warning              = 0xf071,
    wechat               = 0xf1d7,
    weibo                = 0xf18a,
    weixin               = 0xf1d7,
    whatsapp             = 0xf232,
    wheelchair           = 0xf193,
    wifi                 = 0xf1eb,
    wikipediaw           = 0xf266,
    windows              = 0xf17a,
    won                  = 0xf159,
    wordpress            = 0xf19a,
    wrench               = 0xf0ad,
    xing                 = 0xf168,
    xingsquare           = 0xf169,
    ycombinator          = 0xf23b,
    ycombinatorsquare    = 0xf1d4,
    yahoo                = 0xf19e,
    yc                   = 0xf23b,
    ycsquare             = 0xf1d4,
    yelp                 = 0xf1e9,
    yen                  = 0xf157,
    youtube              = 0xf167,
    youtubeplay          = 0xf16a,
    youtubesquare        = 0xf166,

    gitlab = 0xf2a3,
    wpbeginner = 0xf2a3,
    wpforms = 0xf2a2,
    envira = 0xf29e,
    universalaccess = 0xf29d,
    wheelchairalt = 0xf2a1,
    questioncircleo = 0xf2a4,
    blind = 0xf2a4,
    audiodescription = 0xf299,
    volumecontrolphone = 0xf2b4,
    braille = 0xf2b0,
    assistivelisteningsystems = 0xf2b4,
    aslinterpreting = 0xf296,
    americansignlanguageinterpreting = 0xf2a5,
    deafness = 0xf2a6,
    hardofhearing = 0xf2b3,
    deaf = 0xf2b3,
    glide = 0xf2a4,
    glideg = 0xf2a8,
    signing = 0xf2ae,
    signlanguage = 0xf29c,
    lowvision = 0xf2a7,
    viadeo = 0xf2a7,
    viadeosquare = 0xf2ab,
    snapchat = 0xf2ac,
    snapchatghost = 0xf2ad,
    snapchatsquare = 0xf2b2,
    firstorder = 0xf2a9,
    yoast = 0xf2aa,
    themeisle = 0xf2a0,
    googlepluscircle = 0xf29b,
    googleplusofficial = 0xf297,
    fa_ = 0xf298,
    fontawesome = 0xf2b1,

    addresscard = 0xf2bb,
    addresscardo = 0xf2bc,
    bath = 0xf2cd,
    idcard = 0xf2c2,
    idcardo = 0xf2c3,
    s15 = 0xf2cd,
    thermometer = 0xf2c7,
    thermometerempty = 0xf2cb,
    thermometerfull = 0xf2c7,
    thermometerhalf = 0xf2c9,
    thermometerquarter = 0xf2ca,
    thermometerthreequarters = 0xf2c8,
    windowclose = 0xf2d3,
    windowcloseo = 0xf2d4,
    addressbook = 0xf2b9,
    addressbooko = 0xf2ba,
    bandcamp = 0xf2d5,
    bathtub = 0xf2cd,
    driverslicense = 0xf2c2,
    driverslicenseo = 0xf2c3,
    eercast = 0xf2da,
    envelopeopen = 0xf2b6,
    envelopeopeno = 0xf2b7,
    etsy = 0xf2d7,
    freecodecamp = 0xf2c5,
    grav = 0xf2d6,
    handshakeo = 0xf2b5,
    idbadge = 0xf2c1,
    imdb = 0xf2d8,
    linode = 0xf2b8,
    meetup = 0xf2e0,
    microchip = 0xf2db,
    podcast = 0xf2ce,
    quora = 0xf2c4,
    ravelry = 0xf2d9,
    shower = 0xf2cc,
    snowflakeo = 0xf2dc,
    superpowers = 0xf2dd,
    telegram = 0xf2c6,
    thermometer0 = 0xf2cb,
    thermometer1 = 0xf2ca,
    thermometer2 = 0xf2c9,
    thermometer3 = 0xf2c8,
    thermometer4 = 0xf2c7,
    timesrectangle = 0xf2d3,
    timesrectangleo = 0xf2d4,
    usercircle = 0xf2bd,
    usercircleo = 0xf2be,
    usero = 0xf2c0,
    vcard = 0xf2bb,
    vcardo = 0xf2bc,
    windowmaximize = 0xf2d0,
    windowminimize = 0xf2d1,
    windowrestore = 0xf2d2,
    wpexplorer = 0xf2de

  };
};


//---------------------------------------------------------------------------------------

class QtAwesomeIconPainter;

/// The main class for managing icons
/// This class requires a 2-phase construction. You must first create the class and then initialize it via an init* method
class QtAwesome : public QObject
{
Q_OBJECT

public:

    QtAwesome(QObject *parent = 0);
    virtual ~QtAwesome();

    void init( const QString& fontname );
    bool initFontAwesome( const QString &fontAwesomeFile);

    void addNamedCodepoint( const QString& name, fa::iconCodePoint codePoint );
    QHash<QString,fa::iconCodePoint> namedCodePoints() { return namedCodepoints_; }

    void setDefaultOption( const QString& name, const QVariant& value  );
    QVariant defaultOption( const QString& name );

    QIcon icon( fa::iconCodePoint character, const QVariantMap& options = QVariantMap() );
    QIcon icon( const QString& name, const QVariantMap& options = QVariantMap() );
    QIcon icon(QtAwesomeIconPainter* painter, const QVariantMap& optionMap = QVariantMap() );

    void give( const QString& name, QtAwesomeIconPainter* painter );

    QFont font( int size );

    /// Returns the font-name that is used as icon-map
    QString fontName() { return fontName_ ; }

private:
    QString fontName_;                                     ///< The font name used for this map
    QHash<QString,fa::iconCodePoint> namedCodepoints_;                   ///< A map with names mapped to code-points

    QHash<QString, QtAwesomeIconPainter*> painterMap_;     ///< A map of custom painters
    QVariantMap defaultOptions_;                           ///< The default icon options
    QtAwesomeIconPainter* fontIconPainter_;                ///< A special painter fo painting codepoints
};


//---------------------------------------------------------------------------------------


/// The QtAwesomeIconPainter is a specialized painter for painting icons
/// your can implement an iconpainter to create custom font-icon code
class QtAwesomeIconPainter
{
public:
    virtual ~QtAwesomeIconPainter() {}
    virtual void paint( QtAwesome* awesome, QPainter* painter, const QRect& rect, QIcon::Mode mode, QIcon::State state, const QVariantMap& options ) = 0;
};



#endif // QTAWESOME_H
