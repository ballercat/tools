import QtQuick 2.0

Item {
    id: button;
    width: 100;
    height: 100;

    property alias buttonColor: backdrop.color;//"#ff282526";
    property alias color: backdrop.color;
    property alias radius: backdrop.radius;
    property color onHoverColor: "gray";
    property color borderColor: "white";//backdrop.border.color;
    property int borderWidth: 0;//backdrop.border.width;
    property real   textWordSpacing: 0.0;
    property bool useBackdrop : true;
    property bool useHighlight: true;
    property bool toggled : false;
    property bool animated: false;
    property variant mouse : buttonMouseArea;
    default property alias children : content.children;
    property alias icon: icon.source;
    property alias labelWidth: buttonLabel.width;
    property alias font : buttonLabel.font.family;
    property alias pixelSize : buttonLabel.font.pixelSize;
    property alias pointSize: buttonLabel.font.pointSize;
    property alias textColor: buttonLabel.color;
    property alias textWeight: buttonLabel.font.weight;
    property alias label : buttonLabel.text;
    property alias halign: buttonLabel.horizontalAlignment;
    property alias textanchors : buttonLabel.anchors;

    //Visual modifiers
    property bool orientIconLeft : false;
    property bool labelOverIcon : false;
    property bool orientIconRight : false;

    signal buttonClick();
    signal longClick();

    //Behaviors
    Behavior on width { PropertyAnimation { duration: 200 * animated; easing.type: Easing.InOutCirc; }}
    Behavior on height { PropertyAnimation { duration: 200 * animated; easing.type: Easing.InOutCirc; }}

    onOrientIconLeftChanged : {
        if( orientIconLeft ) {
            icon.anchors.centerIn = undefined;
            icon.anchors.left = button.left;
            icon.anchors.verticalCenter = button.verticalCenter;
            icon.anchors.leftMargin = icon.sourceSize.width/4;

            buttonLabel.anchors.centerIn = button;
        }
    }

    onOrientIconRightChanged : {
        if( orientIconRight ) {
            icon.anchors.centerIn = undefined;
            icon.anchors.right = button.right;
            icon.anchors.verticalCenter = button.verticalCenter;
            icon.anchors.rightMargin = icon.sourceSize.width/4;

            buttonLabel.anchors.centerIn = button;
        }
    }

    onButtonClick : {
        /*button.color = Qt.darker(buttonColor, 3.0);
        if(useBackdrop)
            backdrop.color = Qt.darker(button.color, 1.5);
        else
            backdrop.color = button.color;

        buttonLabel.color = Qt.lighter(textColor, 4.0);*/
    }

    Rectangle {
        id: backdrop;
        anchors.fill: parent;
        color: "#00ffffff";
    }


    Image {
        id: icon;
        anchors.centerIn: parent;
        source: "";//button.icon;
        sourceSize.width: 48 * (parent.width/100);
        sourceSize.height: 48 * (parent.height/100);

        Component.onCompleted : {
            if( parent.orientIconLeft ) {
                anchors.centerIn = undefined;
                anchors.left = parent.left;
                anchors.verticalCenter = parent.verticalCenter;
            }
        }
    }





    Text {
        id: buttonLabel;
        font.family: "Helvetica";
        width: parent.width / 2;
        horizontalAlignment: Text.AlignHCenter;
        anchors.bottom: parent.bottom;
        anchors.bottomMargin: parent.height/2 - icon.height*0.8 - buttonLabel.height/2;
        anchors.horizontalCenter: parent.horizontalCenter;
        text: label;
        color: "white";
        wrapMode: Text.WordWrap;
        Component.onCompleted : {
            if( parent.labelOverIcon ) {
                anchors.centerIn = icon;
            }
        }
    }

    MouseArea {
        id: buttonMouseArea;
        anchors.fill: parent;
        onClicked: buttonClick();
        onPressAndHold : longClick();
        hoverEnabled: true;

        property bool revert : false;
        property color prev : "#000000";

        onEntered:{
            revert = true;
            prev = backdrop.color;
            backdrop.color = Qt.darker(backdrop.color, 2); //Qt.lighter("#A00099cc", 2.0);
            //buttonLabel.color = Qt.darker(buttonLabel.color, 1.0);//buttonColor;
        }
        onExited:{
            if( revert ) {
                backdrop.color = prev;
                revert = false;
            }
            //backdrop.color = Qt.lighter(backdrop.color, 2);//buttonColor;
            //buttonLabel.color = Qt.lighter(buttonLabel.color, 1.0);
        }
        onReleased: {
            parent.color = buttonColor;
            backdrop.color = buttonColor;
            buttonLabel.color = textColor;
        }
    }
    Item {
        id: content;
        anchors.fill: parent;
    }
}
