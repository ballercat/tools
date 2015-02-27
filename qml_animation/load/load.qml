import QtQuick 2.4

//https://dribbble.com/shots/1853526-Download

Rectangle {
    id: root;

    property color bar_color : "#1B445D";
    property color bg_color : "#45B0F3";
    property color text_color : "#8696A5";
    property int step_index : 0;
    property int   completed: 0;
    property int indicator_x: root.width/2;
    property int script : 0;
    property int duration : 0;
    property var start_callback : null;
    property var stop_callback : null;

    width: 800;
    height: 600;
    color: bg_color;

    function choose_script()
    {
        //if (script == 0) {
            sucess_script();
        //}
        //script++;
    }

    function sucess_script()
    {
        var i = 0;
        var amount = [ 18, 10, 15, 57 ];//insert -1 to cause failure
        //var time = [ 100, 100, 100, 100 ]; //tests
        var time = [ 800, 1200, 500, 1200 ]; //ms

        completed = 0;
        indicator_x = root.width/2;
        tilt.easing.overshoot = 1.0;
        indicator.x = Qt.binding(function() { return indicator_x - indicator.width/2; });
        text.color = text_color;
        text.text = Qt.binding(function() { return "%" + ((indicator.x - main_bar.x + indicator.width/2)/main_bar.width * 100).toFixed(0);});
        load_bar.width = Qt.binding(function() { return root.completed/100 * main_bar.width; });
        function step() {
            if (root.completed < 100) {
                if (amount[i] < 0) {
                    failure();
                    return;
                }
                root.duration = time[i];
                forward.step = amount[i];

                forward.start();
                tilt.easing.overshoot = 9;//amount[i]/time[i]*400;
                tilt.to = -(amount[i]/2);
                //cap tilt angle to 15;
                if( tilt.to < -15 ) tilt.to = -15;
                tilt.start();
            }
            else {
                //Finish scesss script
                rotate_info.color = "green";
                rotate_info.text = "done";
                root.stop_callback = finish;
                load_bar.width = load_bar.width;
                rotate_info.start();
            }
            i++;
        }

        function failure() {
            rotate_info.color = "red";
            rotate_info.text = "failed";
            root.stop_callback = finish;
            load_bar.width = load_bar.width;
            rotate_info.start();
        }

        function finish() {
            reset.start();
        }

        root.duration = 500;
        root.stop_callback = step;
        rewind.start();
    }


    Rectangle {
        id: main_bar;
        color: bar_color;
        width: root.width * 0.15;
        height: root.height * 0.2;
        radius: 5;
        anchors.horizontalCenter: parent.horizontalCenter;
        y: parent.height/2 - height/2;


        ParallelAnimation {
            id: into_loadbar;
            running: false;
            NumberAnimation { target: main_bar; property: "width"; to: root.width * 0.7; duration: 450;
            easing.type: Easing.OutBack;
                easing.overshoot: 4;
            }
            NumberAnimation { target: main_bar; property: "height"; to: 7; duration: 150; }

            onStopped : {

            }
        }

        MouseArea {
            anchors.fill: parent;
            onClicked :{
                into_loadbar.start();
                indicator_shrink.start();
                mouse.accepted = true;
            }
        }
    }

    Rectangle {
        id: load_bar;
        color: "white";
        x: main_bar.x;
        y: main_bar.y;
        height: main_bar.height;
        radius: 5;
        width: root.completed/100 * main_bar.width;

        Behavior on width { NumberAnimation{ duration: forward.duration; easing.type: forward.easing.type; }}
    }

    Canvas {
        id: arrow;
        contextType: "2d";
        anchors.top: indicator.bottom; anchors.topMargin: -height*0.22;
        anchors.horizontalCenter: indicator.horizontalCenter;
        width: root.width * 0.08;
        height: width;
        Path {
            id: tpath;
            startX: 0; startY: 0;
            PathLine { relativeX: arrow.width/2; relativeY: arrow.height/2; }
            PathLine { relativeX: arrow.width/2; relativeY: -arrow.height/2; }
        }

        onPaint : {
            context.fillStyle = Qt.rgba(1.0, 1.0, 1.0);
            context.path = tpath;
            context.fill();
        }
    }

    Rectangle {
        id: indicator;
        color: "white";
        width: root.width * 0.04;
        height: root.height * 0.08;
        radius: 5;
        x: indicator_x - width/2;
        y: root.height/2 - root.height * 0.05;

        PropertyAnimation {
            id: tilt;
            target: indicator;
            property: "rotation";
            duration: root.duration/5;
            to: 0;
            easing.type: Easing.InOutBack;
        }

            Timer {
                id: pause;
                interval: root.duration/5; running: false; repeat: false;

                onRunningChanged : {
                    if (running) {
                        tilt.start();
                    }
                }

                onTriggered : {
                    root.stop_callback();
                }
            }

        ParallelAnimation {
            id: indicator_shrink;
            running: false;
            NumberAnimation { target: indicator; properties: "height"; to: root.height * 0.045; duration: 400 }
            NumberAnimation { target: indicator; properties: "width"; to: root.height * 0.08; duration: 400 }
            NumberAnimation {
                target: indicator;
                property: "y";
                to: root.height/2 - root.height * 0.06;//.235;
                duration: 300;
                easing.overshoot: 100.0;
                easing.type: Easing.OutBack;
            }
            NumberAnimation { target: arrow; properties: "width"; to: root.width * 0.02; duration: 50; }
            onStopped : choose_script();
        }

        ParallelAnimation {
            id: reset;
            running: false;
            PathAnimation {
                target: indicator;
                duration: 680;
                easing.type: Easing.InCirc;
                path : Path {
                    startX: indicator.x;
                    startY: indicator.y;
                    PathArc { x: (root.width/2 - root.width*0.02);
                                y: (root.height/2 - root.height*0.05);
                                direction: (indicator.x > root.width/2) ? PathArc.Counterclockwise : PathArc.Clockwise;
                                radiusX: 150;
                                radiusY: 350;
                    }
                }
            }
            NumberAnimation { target: arrow; properties: "width"; to: root.width * 0.08; duration: 600; easing.type:Easing.InQuad; }
            NumberAnimation { target: arrow; properties: "opacity"; to: 1.0; duration: 500; }

            NumberAnimation { target: indicator; properties: "width"; to: root.width * 0.04; duration: 400;}
            NumberAnimation { target: indicator; properties: "height"; to: root.height * 0.08; duration: 400;}

            NumberAnimation { target: main_bar; properties: "width"; to: root.width * 0.15; duration: 700;
                easing.type: Easing.OutQuint;
            }
            NumberAnimation { target: main_bar; properties: "height"; to: root.height * 0.2; duration : 700;
                easing.type: Easing.OutQuint;
            }
            onStarted: {
                root.duration = 100;
                root.completed = 0;
            }
        }

        SequentialAnimation {
            id: rotate_info;
            running: false;
            property string text: ""
            property color color :"green";
            ParallelAnimation {
                ParallelAnimation {
                    NumberAnimation { target: load_bar; property: "x"; to: indicator.x; duration: 300; }
                    NumberAnimation { target: load_bar; property: "width"; to: 0; duration: 315; }
                }
                NumberAnimation { target: indicator; properties: "width"; to: 0;/*root.width  * 0.03*/ duration: 300; }
                NumberAnimation { target: arrow; property: "width"; to: 1; duration: 300; }
            }
            ParallelAnimation {
                NumberAnimation { target: indicator; properties: "width"; to: root.height * 0.09; duration: 150; }
                NumberAnimation { target: arrow; property: "width"; to: root.width * 0.02; duration: 150; }
            }
            onStarted : {
                text.opacity = 0.0;
            }

            onStopped : {
                text.opacity = 1.0;
                text.color = "green";
                text.text = "done"
                indicator.x = indicator.x;
                root.duration = 4000;
                pause.start();
            }
        }

        Text {
            id: text;
            anchors.centerIn: parent;
            visible: (root.completed > 0);
            text: "%" + ((indicator.x - main_bar.x + indicator.width/2)/main_bar.width * 100).toFixed(0);
            rotation: indicator.rotation;
            font.pixelSize: parent.height * 0.5;
            color: root.text_color;

            Behavior on opacity { NumberAnimation{ duration: 100; } }
        }
    }

    PropertyAnimation {
        id: rewind;
        target: root;
        property: "indicator_x";
        to: main_bar.x;
        easing.type: Easing.InOutQuad;

        onStarted : {
            tilt.to = 10;
            tilt.start();
        }

        onStopped : {
            root.duration = 500;
            tilt.to = 0;
            pause.start();
        }
    }

    PropertyAnimation {
        id: forward;
        target: root;
        property: "indicator_x";
        property int step: 0;
        to: main_bar.x + (root.completed + step) * main_bar.width/100;
        duration: root.duration;
        easing.type: Easing.Linear;

        onStarted : {
            root.completed += step;
        }

        onStopped : {
            tilt.to = 0;
            pause.start();
        }
    }
}
