import QtQuick 2.5
import QtQuick.Dialogs 1.2
import QtQuick.Controls 1.4
import QtQuick.Window 2.2

Window {
    id: about
    title: "About RespeQt"
    width: 800
    height: 600

    Column {
        anchors.fill: parent

        Label {
            text: "RespeQt: Atari Serial Peripheral Emulator for Qt"
            font.pointSize: 12
            font.bold: true
            width: parent.width
            horizontalAlignment: Text.AlignHCenter
        }

        Label {
            font.pointSize: 10
            width: parent.width
            text: "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.01 Transitional//EN\" \"http://www.w3.org/TR/html4/loose.dtd\">\n" +
                "<html>\n" +
                "<meta content=\"en-us\" http-equiv=\"Content-Language\" />\n" +
                "<meta content=\"text/html; charset=utf-8\" http-equiv=\"Content-Type\" />\n" +
                "<title>RespeQt</title>\n" +
                "<style type=\"text/css\">\n" +
                "body { font-family: Arial, Helvetica, sans-serif; }\n" +
                "h1   { color: #4C920E; font-size: large; }\n" +
                "</style>\n" +
                "<body>\n" +
                "\n" +
                "\t<h1><img src=\"/icons/main-icon/RespeQt.png\" align=\"middle\"/>&nbsp;RespeQt, Atari Serial Peripheral Emulator for Qt</h1>\n" +
                "\t\n" +
                "\t<p align=\"justify\"><b>Summary</b><br />RespeQt emulates Atari SIO peripherals when connected to an Atari 8-bit computer with an SIO2PC cable. \n" +
                "\tIn that respect t's similar to programs like APE, Atari810, and AspeQt. The main difference is that it's free (unlike \n" +
                "\tAPE and AspeQt) and it's cross-platform (unlike Atari810 and APE).</p>\n" +
                "\t\n" +
                "\t<br /><br /><b>Some features</b>\n" +
                "\t\n" +
                "\t<ul type=\"square\">\n" +
                "\t\t<li>Qt based GUI with drag and drop support.</li>\n" +
                "\t\t<li>Cross-platform. <em>(Windows, Linux, OSX)</em></li>\n" +
                "\t\t<li>15 disk drive emulation. <em>(drives 9-15 are only supported by SpartaDOS X)</em></li>\n" +
                "\t\t<li>Text-only printer emulation with saving and printing of the output.</li>\n" +
                "\t\t<li>Cassette image playback.</li>\n" +
                "\t\t<li>Folders can be mounted as simulated Dos20s disks. <em>(read-only, now with SDX compatibility, and bootable)</em></li>\n" +
                "\t\t<li>Atari executables can be booted directly, optionally with high speed.</li>\n" +
                "\t\t<li>Contents of image files can be viewed / changed.</li>\n" +
                "\t\t<li>RespeQt Client module RCL.COM. Runs on the Atari and is used to get/set Date/Time on the Atari plus a variety of other remote tasks.</li>\n" +
                "\t\t<li>ApeTime client support.</li>\n" +
                "\t\t<li>Up to 6xSIO speed and more if the serial port adaptor supports it <em>(FTDI chip based cables are recommanded).</em></li>\n" +
                "\t\t<li>Localization support (Currently for English, German, Polish, Russian, Slovak, Spanish and Turkish). ***this may be broken***</li>\n" +
                "\t\t<li>Multi-session support.</li>\n" +
                "\t\t<li>PRO and ATX images support in read/write mode with accurate (I hope so) protection emulation.</li>\n" +
                "\t\t<li>Spy mode to display data exchanged with Atari.</li>\n" +
                "\t\t<li>Happy 810 Rev.5 and Happy 1050 Rev.7 emulation to read/write real floppy disks from/to images.</li>\n" +
                "\t\t<li>Chip 810 and Super Archiver 1050 emulation to read/write real floppy disks from/to images.</li>\n" +
                "\t\t<li>Automatic Translator activation when OS-B is detected in the filename in drive D1:</li>\n" +
                "\t\t<li>Favorite tool disk accessible in one click in drive D1:</li>\n" +
                "        </ul>\n" +
                "\t\n" +
                "\t<p align=\"justify\"><b>License</b><br>\n" +
                "        RespeQt fork <em>Copyright 2015-2017 by Joseph Zatarski</em>, <em>Copyright 2015 by DrVenkman</em>, <em>Copyright 2016, 2017 by TheMontezuma</em>, <em>Copyright 2016, 2017 by Jochen Sch&auml;fer (josch1710)</em>, and <em>Copyright 2017 by blind</em>.<br />\n" +
                "\tRespeQt enhancements <em>Copyright 2018 by ebiguy</em><br />\n" +
                "\tRespeQt is based on AspeQt 1.0.0-preview7<br />\n" +
                "\tOriginal AspeQt code up to version 0.6.0 <em>Copyright 2009 by Fatih Aygün.</em><br />\n" +
                "\tUpdates to AspeQt since v0.6.0 to 1.0.0-preview7 <em>Copyright 2012 by Ray Ataergin</em><br />\n" +
                "\tYou can freely copy, use, modify and distribute RespeQt under the GPL 2.0 license. Please see license.txt for details.<br />\n" +
                "\tQt libraries: <em>Copyright 2009 Nokia Corporation</em> and/or its subsidiary(-ies). Used in this package under LGPL 2.0 license.<br />\n" +
                "\tSilk Icons: <em>Copyright <a href=\"www.famfamfam.com\">Mark James</a>.</em>Used in this package under Creative Commons Attribution 3.0 license.<br />\n" +
                "\tAdditional Icons by <em><a href=\"http://www.oxygen-icons.org/\">Oxygen Team</a></em>.<br /> \n" +
                "\tUsed in this package under Creative Commons Attribution-ShareAlike 3.0 license.<br />\n" +
                "\tAtariSIO Linux kernel module and high speed code used in the EXE loader Copyright <a href=\"http://www.horus.com/~hias/atari/\">Matthias Reichl.</a><br />\n" +
                "\tUsed in this package under GPL 2.0 license.<br />\n" +
                "\tAtascii Fonts by <em><a href=\"http://members.bitstream.net/~marksim/atarimac\">Mark Simonson</a></em>. Used in this package under Freeware License.<br />\n" +
                "        1020 emu font by <em><a href=\"http://atariage.com/forums/user/7682-unixcoffee928/\">UNIXcoffee928</a></em>. Used with permission under GPL 2.0 license.<br />\n" +
                "        RespeQt icon by <em>djmat56.</em> Used with permission.<br />\n" +
                "\tSupport for the spanish language by AsCrNet.<br />\n" +
                "\tRespeQt Client program (rcl.com) Copyright FlashJazzCat. Used under the GPL v2.\n" +
                "\t</p>\n" +
                "\t\n" +
                "\t<p align=\"justify\"><b>DOS files distributed with RespeQt</b> are copyright of their respective owners, Joseph Zatarski and RespeQt distributes those files with the understanding that they are either abandonware or public domain, and are widely available for download through the internet. If you are the copyright holder of one or more of these files, and believe that distribution of these files constitutes a breach of your rights please contact ebiguy on AtariAge. We respect the rights of copyright holders and won't distribute copyrighted work without the rights holder's consent.</p>\n" +
                "\t\n" +
                "\t<p align=\"justify\"><b>Special Thanks</b><br/>\n" +
                "\tKyle22 for finding 2 instances of AspeQt, 1 reference to Ray Ataergin, 1 reference to Ray's website which needed to be changed, for finding that the about and user documentation dialogs were out of date (way out of date), contact information in readme.txt was out of date, and for pointing out that I forgot to update the documentation for r1.<br />\n" +
                "\tFlashJazzCat for writing a replacement for AspeCL.</p>\n" +
                "\t\n" +
                "        <p><b>Contact</b><br />ebiguy on AtariAge,<em>Please include the word \"Respeqt\" in the subject field.</em></p>\n" +
                "\n" +
                "</body>\n" +
                "</html>\n"
        }
    }
}
