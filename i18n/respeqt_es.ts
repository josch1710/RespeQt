<?xml version="1.0" encoding="utf-8"?>
<!DOCTYPE TS>
<TS version="2.1" language="es_ES">
<context>
    <name>AboutDialog</name>
    <message>
        <location filename="../ui/aboutdialog.ui" line="14"/>
        <source>About RespeQt</source>
        <translation>Acerca de RespeQt</translation>
    </message>
    <message>
        <location filename="../ui/aboutdialog.ui" line="32"/>
        <source>RespeQt: Atari Serial Peripheral Emulator for Qt</source>
        <translation>RespeQt: Atari Emulador Serial de Periférico para Qt</translation>
    </message>
    <message>
        <location filename="../ui/aboutdialog.ui" line="75"/>
        <location filename="../src/aboutdialog.cpp" line="22"/>
        <source>qrc:/documentation/about.html</source>
        <translation>qrc:/documentation/about_spanish.html</translation>
    </message>
    <message>
        <location filename="../ui/aboutdialog.ui" line="98"/>
        <source>About Qt</source>
        <translation>Acerca de Qt</translation>
    </message>
    <message>
        <location filename="../src/aboutdialog.cpp" line="21"/>
        <source>version %1</source>
        <translation>versión %1</translation>
    </message>
</context>
<context>
    <name>AtariSioBackend</name>
    <message>
        <location filename="../src/serialport-unix.cpp" line="676"/>
        <source>Cannot open serial port &apos;%1&apos;: %2</source>
        <translation>No se puede abrir el puerto serial &apos;%1&apos;: %2</translation>
    </message>
    <message>
        <location filename="../src/serialport-unix.cpp" line="684"/>
        <location filename="../src/serialport-unix.cpp" line="691"/>
        <source>Cannot open AtariSio driver &apos;%1&apos;: %2</source>
        <translation>No se puede abrir el controlador AtariSio &apos;%1&apos;: %2</translation>
    </message>
    <message>
        <location filename="../src/serialport-unix.cpp" line="714"/>
        <source>Cannot set AtariSio driver mode: %1</source>
        <translation>No se puede establecer conexión con AtariSio: %1</translation>
    </message>
    <message>
        <location filename="../src/serialport-unix.cpp" line="720"/>
        <source>Cannot set AtariSio to autobaud mode: %1</source>
        <translation>No se puede establecer conexión con AtariSio para modo autobaudio %1</translation>
    </message>
    <message>
        <location filename="../src/serialport-unix.cpp" line="726"/>
        <source>Cannot create the cancel pipe</source>
        <translation>No se puede crear la tubería</translation>
    </message>
    <message>
        <location filename="../src/serialport-unix.cpp" line="744"/>
        <source>Emulation started through AtariSIO backend on &apos;%1&apos; with %2 handshaking.</source>
        <translation>Emulación inicia a través de backend en AtariSIO &apos;%1&apos; con el %2 handshaking.</translation>
    </message>
    <message>
        <location filename="../src/serialport-unix.cpp" line="756"/>
        <source>Cannot close serial port: %1</source>
        <translation>No se puede cerrar el puerto serial: %1</translation>
    </message>
    <message>
        <location filename="../src/serialport-unix.cpp" line="765"/>
        <source>Cannot stop AtariSio backend.</source>
        <translation>No se puede dejar de backend AtariSio.</translation>
    </message>
    <message>
        <location filename="../src/serialport-unix.cpp" line="774"/>
        <source>Cannot set AtariSio speed to %1: %2</source>
        <translation>No se puede establecer la velocidad de AtariSio %1: %2</translation>
    </message>
    <message>
        <location filename="../src/serialport-unix.cpp" line="777"/>
        <location filename="../src/serialport-unix.cpp" line="827"/>
        <source>%1 bits/sec</source>
        <translation>%1 bits/sec</translation>
    </message>
    <message>
        <location filename="../src/serialport-unix.cpp" line="778"/>
        <location filename="../src/serialport-unix.cpp" line="828"/>
        <source>Serial port speed set to %1.</source>
        <translation>Velocidad del puerto serial configurado para %1.</translation>
    </message>
    <message>
        <location filename="../src/serialport-unix.cpp" line="834"/>
        <source>Illegal condition using select!</source>
        <translation>Condición ilegal usando select!</translation>
    </message>
    <message>
        <location filename="../src/serialport-unix.cpp" line="866"/>
        <source>Cannot read data frame: %1</source>
        <translation>No se puede leer trama de datos: %1</translation>
    </message>
    <message>
        <location filename="../src/serialport-unix.cpp" line="881"/>
        <source>Cannot write data frame: %1</source>
        <translation>No se puede escribir trama de datos: %1</translation>
    </message>
    <message>
        <location filename="../src/serialport-unix.cpp" line="891"/>
        <source>Cannot write command ACK: %1</source>
        <translation>No se puede escribir el comando ACK: %1</translation>
    </message>
    <message>
        <location filename="../src/serialport-unix.cpp" line="900"/>
        <source>Cannot write command NAK: %1</source>
        <translation>No se puede escribir el comando NAK: %1</translation>
    </message>
    <message>
        <location filename="../src/serialport-unix.cpp" line="908"/>
        <source>Cannot write data ACK: %1</source>
        <translation>No se puede escribir datos ACK: %1</translation>
    </message>
    <message>
        <location filename="../src/serialport-unix.cpp" line="916"/>
        <source>Cannot write data NAK: %1</source>
        <translation>No se puede escribir datos de NAK: %1</translation>
    </message>
    <message>
        <location filename="../src/serialport-unix.cpp" line="924"/>
        <source>Cannot write COMPLETE byte: %1</source>
        <translation>No se puede escribir, byte COMPLETO: %1</translation>
    </message>
    <message>
        <location filename="../src/serialport-unix.cpp" line="932"/>
        <source>Cannot write ERROR byte: %1</source>
        <translation>No se puede escribir, byte ERROR : %1</translation>
    </message>
    <message>
        <location filename="../src/serialport-unix.cpp" line="945"/>
        <source>Cannot write raw frame: %1</source>
        <translation>No se puede escribir a marco bruto: %1</translation>
    </message>
    <message>
        <location filename="../src/serialport-unix.cpp" line="955"/>
        <source>Block too long.</source>
        <translation>Bloque demasiado largo.</translation>
    </message>
    <message>
        <location filename="../src/serialport-unix.cpp" line="958"/>
        <source>Command not acknowledged.</source>
        <translation>Comando no reconocido.</translation>
    </message>
    <message>
        <location filename="../src/serialport-unix.cpp" line="961"/>
        <source>Command timeout.</source>
        <translation>Tiempo de espera de comando.</translation>
    </message>
    <message>
        <location filename="../src/serialport-unix.cpp" line="964"/>
        <source>Checksum error.</source>
        <translation>Checksum error.</translation>
    </message>
    <message>
        <location filename="../src/serialport-unix.cpp" line="967"/>
        <source>Device error.</source>
        <translation>Error en dispositivo.</translation>
    </message>
    <message>
        <location filename="../src/serialport-unix.cpp" line="970"/>
        <source>Data frame not acknowledged.</source>
        <translation>No reconoció trama de datos.</translation>
    </message>
    <message>
        <location filename="../src/serialport-unix.cpp" line="973"/>
        <source>Unknown AtariSio driver error.</source>
        <translation>Error desconocido con AtariSio.</translation>
    </message>
    <message>
        <location filename="../src/serialport-win32.cpp" line="680"/>
        <source>AtariSIO is only available under Linux.</source>
        <translation>AtariSIO sólo está disponible en Linux.</translation>
    </message>
</context>
<context>
    <name>AutoBoot</name>
    <message>
        <location filename="../src/autoboot.cpp" line="26"/>
        <source>[%1] command: $%2, aux: $%3 NAKed.</source>
        <translation>[%1] comando: $%2, aux: $%3 NAKed.</translation>
    </message>
    <message>
        <location filename="../src/autoboot.cpp" line="42"/>
        <source>[%1] Speed poll.</source>
        <translation>[%1] Velocidad de sondeo.</translation>
    </message>
    <message>
        <location filename="../src/autoboot.cpp" line="63"/>
        <source>[%1] Read sector %2 (%3 bytes).</source>
        <translation>[%1] Leyendo sector %2 (%3 bytes).</translation>
    </message>
    <message>
        <location filename="../src/autoboot.cpp" line="83"/>
        <source>[%1] Get status.</source>
        <translation>[%1] Obtener el estado.</translation>
    </message>
    <message>
        <location filename="../src/autoboot.cpp" line="90"/>
        <source>[%1] Atari is jumping to %2.</source>
        <translation>[%1] Atari está saltando a %2.</translation>
    </message>
    <message>
        <location filename="../src/autoboot.cpp" line="98"/>
        <source>[%1] Invalid chunk in get chunk: aux = %2</source>
        <translation>[%1] Parte inválida en obtener: aux = %2</translation>
    </message>
    <message>
        <location filename="../src/autoboot.cpp" line="105"/>
        <source>[%1] Get chunk %2 (%3 bytes).</source>
        <translation>[%1] Obtener trozo %2 (%3 bytes).</translation>
    </message>
    <message>
        <location filename="../src/autoboot.cpp" line="114"/>
        <source>[%1] Invalid chunk in get chunk info: aux = %2</source>
        <translation>[%1] Información no válida en obtener información de la parte:  aux = %2</translation>
    </message>
    <message>
        <location filename="../src/autoboot.cpp" line="132"/>
        <source>[%1] Get chunk info %2 (%3 bytes at %4).</source>
        <translation>[%1] Obtén información del trozo %2 (%3 bytes en %4).</translation>
    </message>
    <message>
        <location filename="../src/autoboot.cpp" line="147"/>
        <source>Cannot open file &apos;%1&apos;: %2</source>
        <translation>No se puede abrir el archivo &apos;%1&apos;: %2</translation>
    </message>
    <message>
        <location filename="../src/autoboot.cpp" line="160"/>
        <location filename="../src/autoboot.cpp" line="179"/>
        <source>Unexpected end of file, needed %1 more</source>
        <translation>Fin de archivo inesperado, necesitaba un %1 más</translation>
    </message>
    <message>
        <location filename="../src/autoboot.cpp" line="164"/>
        <location filename="../src/autoboot.cpp" line="183"/>
        <location filename="../src/autoboot.cpp" line="202"/>
        <location filename="../src/autoboot.cpp" line="224"/>
        <location filename="../src/autoboot.cpp" line="250"/>
        <location filename="../src/autoboot.cpp" line="264"/>
        <source>Cannot read from file &apos;%1&apos;: %2.</source>
        <translation>No se puede leer el archivo &apos;%1&apos;: %2.</translation>
    </message>
    <message>
        <location filename="../src/autoboot.cpp" line="170"/>
        <source>Cannot load file &apos;%1&apos;: The file doesn&apos;t seem to be an Atari DOS executable.</source>
        <translation>No se puede cargar el archivo &apos;%1&apos;: El archivo no parece ser un archivo ejecutable de Atari DOS.</translation>
    </message>
    <message>
        <location filename="../src/autoboot.cpp" line="195"/>
        <location filename="../src/autoboot.cpp" line="222"/>
        <location filename="../src/autoboot.cpp" line="247"/>
        <location filename="../src/autoboot.cpp" line="261"/>
        <source>The executable &apos;%1&apos; is broken: Unexpected end of file, needed %2 more.</source>
        <translation>El ejecutable &apos;%1&apos; está roto: fin de archivo inesperado, necesitaba un %2 más.</translation>
    </message>
    <message>
        <location filename="../src/autoboot.cpp" line="212"/>
        <source>The executable &apos;%1&apos; is broken: The end address is less than the start address.</source>
        <translation>El ejecutable &apos;%1&apos; está roto: La dirección final es menor que la dirección de inicio.</translation>
    </message>
    <message>
        <location filename="../src/autoboot.cpp" line="285"/>
        <source>Cannot open the boot loader: %1</source>
        <translation>No se puede abrir el gestor de arranque: %1</translation>
    </message>
</context>
<context>
    <name>AutoBootDialog</name>
    <message>
        <location filename="../ui/autobootdialog.ui" line="26"/>
        <source>Boot executable</source>
        <translation>Inicio archivo ejecutable</translation>
    </message>
    <message>
        <location filename="../ui/autobootdialog.ui" line="87"/>
        <source>Please reboot your Atari.

Don&apos;t forget to remove any cartridges and disable BASIC by holding down the Atari Option button if necessary.</source>
        <translation>Por favor reinicie su Atari.

No olvide retirar los cartuchos y deshabilitar BASIC manteniendo presionado el botón de Opción Atari si es necesario.</translation>
    </message>
    <message>
        <location filename="../ui/autobootdialog.ui" line="117"/>
        <source>Use this button to re-load the executable 
if it has been changed since the last re-boot 
of your Atari computer - Useful for developers </source>
        <translation>Utilice este botón para volver a cargar el ejecutable
si se ha cambiado desde el último reinicio
de su computadora Atari - Útil para desarrolladores </translation>
    </message>
    <message>
        <location filename="../ui/autobootdialog.ui" line="122"/>
        <source>Reload</source>
        <translation>Recargar</translation>
    </message>
    <message>
        <location filename="../src/autobootdialog.cpp" line="52"/>
        <source>Atari is loading the booter.</source>
        <translation>Atari está cargando el gestor de arranque.</translation>
    </message>
    <message>
        <location filename="../src/autobootdialog.cpp" line="56"/>
        <source>Atari is loading the program.

For some programs you may have to close this dialog manually when the program starts.</source>
        <translation>Atari está cargando el programa.

En algunos programas es posible que tenga que cerrar este diálogo manualmente cuando se inicia el programa.</translation>
    </message>
</context>
<context>
    <name>BootOptionsDialog</name>
    <message>
        <location filename="../ui/bootoptionsdialog.ui" line="26"/>
        <source>Folder Boot Options</source>
        <translation>Selecciona carpeta de boteo</translation>
    </message>
    <message>
        <location filename="../ui/bootoptionsdialog.ui" line="60"/>
        <source>Folder Image Boot Options</source>
        <translation>Selecciona Imagen de boteo desde carpeta</translation>
    </message>
    <message>
        <location filename="../ui/bootoptionsdialog.ui" line="150"/>
        <source>Select the DOS you want to boot your Atari with</source>
        <translation>Seleccione algun DOS que desea arrancar con el ATARI</translation>
    </message>
    <message>
        <location filename="../ui/bootoptionsdialog.ui" line="69"/>
        <source>SmartDOS 6.1D</source>
        <translation></translation>
    </message>
    <message>
        <location filename="../ui/bootoptionsdialog.ui" line="85"/>
        <source>SpartaDOS 3.2G</source>
        <translation></translation>
    </message>
    <message>
        <location filename="../ui/bootoptionsdialog.ui" line="99"/>
        <source>(Check if you&apos;re already using a high-speed OS)</source>
        <translation>(Compruebe si ya está utilizando un sistema operativo de alta velocidad)</translation>
    </message>
    <message>
        <location filename="../ui/bootoptionsdialog.ui" line="115"/>
        <source>MyDOS 4.x</source>
        <translation></translation>
    </message>
    <message>
        <location filename="../ui/bootoptionsdialog.ui" line="131"/>
        <source>AtariDOS 2.x</source>
        <translation></translation>
    </message>
    <message>
        <location filename="../ui/bootoptionsdialog.ui" line="166"/>
        <source>MyPicoDOS 4.05 (Standard)</source>
        <translation></translation>
    </message>
    <message>
        <location filename="../ui/bootoptionsdialog.ui" line="176"/>
        <source>DosXL 2.x</source>
        <translation></translation>
    </message>
    <message>
        <location filename="../ui/bootoptionsdialog.ui" line="192"/>
        <source>Disable high speed SIO</source>
        <translation>Desactivar SIO de alta velocidad</translation>
    </message>
</context>
<context>
    <name>CassetteDialog</name>
    <message>
        <location filename="../ui/cassettedialog.ui" line="23"/>
        <source>Cassette image playback</source>
        <translation>Cassette de reproducción de imágenes</translation>
    </message>
    <message>
        <location filename="../ui/cassettedialog.ui" line="35"/>
        <source>Do whatever is necessary in your Atari to load this cassette image like rebooting while holding Option and Start buttons or entering &quot;CLOAD&quot; in the BASIC prompt.

When you hear the beep sound, push the OK button below and press a key on your Atari at about the same time.</source>
        <translation>Haga lo que sea necesario en su Atari para cargar esta imagen como reiniciar casete mientras se mantiene la opción y botones de arranque o la introducción de &quot;CLOAD&quot; en el símbolo del sistema BASIC.

Cuando oiga el pitido, pulse el botón OK y presione una tecla en el Atari al mismo tiempo.</translation>
    </message>
    <message>
        <location filename="../src/cassettedialog.cpp" line="35"/>
        <source>RespeQt is ready to playback the cassette image file &apos;%1&apos;.

The estimated playback duration is: %2:%3

Do whatever is necessary in your Atari to load this cassette image like rebooting while holding Option and Start buttons or entering &quot;CLOAD&quot; in the BASIC prompt.

When you hear the beep sound, push the OK button below and press a key on your Atari at about the same time.</source>
        <translation>RespeQt está listo para reproducir el archivo de imagen de casete &apos;%1&apos;.

La duración de reproducción estimada es: %2:%3

Cuando oiga el pitido, pulse el botón OK y presione una tecla en el Atari al mismo tiempo.</translation>
    </message>
    <message>
        <location filename="../src/cassettedialog.cpp" line="88"/>
        <location filename="../src/cassettedialog.cpp" line="104"/>
        <source>Playing back cassette image.

Estimated time left: %1:%2</source>
        <translation>Reproducción de imagen cassette..

Tiempo restante estimado: %1:%2</translation>
    </message>
</context>
<context>
    <name>CassetteWorker</name>
    <message>
        <location filename="../src/sioworker.cpp" line="522"/>
        <source>Cannot open &apos;%1&apos;: %2</source>
        <translation>No se puede abrir &apos;%1&apos;: %2</translation>
    </message>
    <message>
        <location filename="../src/sioworker.cpp" line="533"/>
        <location filename="../src/sioworker.cpp" line="542"/>
        <location filename="../src/sioworker.cpp" line="564"/>
        <location filename="../src/sioworker.cpp" line="574"/>
        <source>Cannot read &apos;%1&apos;: %2</source>
        <translation>No se puede leer &apos;%1&apos;: %2</translation>
    </message>
    <message>
        <location filename="../src/sioworker.cpp" line="548"/>
        <source>Cannot open &apos;%1&apos;: The header does not match.</source>
        <translation>No se puede abrir &apos;%1&apos;: El encabezado no coincide.</translation>
    </message>
    <message>
        <location filename="../src/sioworker.cpp" line="553"/>
        <source>[Cassette]: File description &apos;%2&apos;.</source>
        <translation>[Casete]: Descripción del archivo &apos;%2&apos;.</translation>
    </message>
    <message>
        <location filename="../src/sioworker.cpp" line="594"/>
        <source>Cannot open &apos;%1&apos;: Unknown chunk header %2.</source>
        <translation>No se puede abrir &apos;%1&apos;: Encabezado mal formado %2.</translation>
    </message>
    <message>
        <location filename="../src/sioworker.cpp" line="640"/>
        <source>[Cassette] Playing record %1 of %2 (%3 ms of gap + %4 bytes of data)</source>
        <translation>[Casete] Reproducción de registro %1 de %2 (%3 ms de gap + %4 bytes de datos)</translation>
    </message>
</context>
<context>
    <name>CreateImageDialog</name>
    <message>
        <location filename="../ui/createimagedialog.ui" line="23"/>
        <source>Create a disk image</source>
        <translation>Crear una imagen de disco</translation>
    </message>
    <message>
        <location filename="../ui/createimagedialog.ui" line="53"/>
        <source>Standard single density</source>
        <translation>Densidad Individual estandard</translation>
    </message>
    <message>
        <location filename="../ui/createimagedialog.ui" line="63"/>
        <source>Standard enhanced (also called medium or dual) density</source>
        <translation>Estándar mejorada (también llamado medio o doble) Densidad</translation>
    </message>
    <message>
        <location filename="../ui/createimagedialog.ui" line="70"/>
        <source>Standard double density</source>
        <translation>Densidad doble estándar</translation>
    </message>
    <message>
        <location filename="../ui/createimagedialog.ui" line="77"/>
        <source>Double sided double density</source>
        <translation>Doble densidad de doble cara</translation>
    </message>
    <message>
        <location filename="../ui/createimagedialog.ui" line="84"/>
        <source>Double density hard disk</source>
        <translation>Disco duro de doble densidad</translation>
    </message>
    <message>
        <location filename="../ui/createimagedialog.ui" line="91"/>
        <source>Custom</source>
        <translation>A la medida</translation>
    </message>
    <message>
        <location filename="../ui/createimagedialog.ui" line="118"/>
        <source>Number of sectors:</source>
        <translation>Número de sectores:</translation>
    </message>
    <message>
        <location filename="../ui/createimagedialog.ui" line="142"/>
        <source>Sector density:</source>
        <translation>Sector densidad:</translation>
    </message>
    <message>
        <location filename="../ui/createimagedialog.ui" line="150"/>
        <source>Single (128 bytes per sector)</source>
        <translation>Normal (128 bytes por sector)</translation>
    </message>
    <message>
        <location filename="../ui/createimagedialog.ui" line="155"/>
        <source>Double (256 bytes per sector)</source>
        <translation>Doble (256 bytes por sector)</translation>
    </message>
    <message>
        <location filename="../ui/createimagedialog.ui" line="160"/>
        <source>512 bytes per sector</source>
        <translation>512 bytes por sector</translation>
    </message>
    <message>
        <location filename="../ui/createimagedialog.ui" line="165"/>
        <source>8192 bytes per sector</source>
        <translation>8192 bytes por sector</translation>
    </message>
    <message>
        <location filename="../ui/createimagedialog.ui" line="181"/>
        <source>Total image capacity: 92160 bytes (90 K)</source>
        <translation>Capacidad de imagen Total: 92160 bytes (90 KB)</translation>
    </message>
    <message>
        <location filename="../src/createimagedialog.cpp" line="87"/>
        <source>Total image capacity: %1 bytes (%2 K)</source>
        <translation>Capacidad de imagen total: %1 bytes (%2 K)</translation>
    </message>
</context>
<context>
    <name>DiskBrowserDlg</name>
    <message>
        <location filename="../ui/diskbrowserdlg.ui" line="14"/>
        <source>Disk Collection Browser</source>
        <translation>Explorador de colecciones de discos</translation>
    </message>
    <message>
        <location filename="../ui/diskbrowserdlg.ui" line="38"/>
        <source>Browse</source>
        <translation>Examinar</translation>
    </message>
    <message>
        <location filename="../src/diskbrowser/diskbrowserdlg.cpp" line="99"/>
        <source>Open folder containing disk images</source>
        <translation>Abrir carpeta que contiene imágenes de disco</translation>
    </message>
    <message>
        <location filename="../src/diskbrowser/diskbrowserdlg.cpp" line="272"/>
        <source>!Root dir empty:
No Files</source>
        <translation>Directorio raíz vacío:
No hay archivos</translation>
    </message>
    <message>
        <location filename="../src/diskbrowser/diskbrowserdlg.cpp" line="276"/>
        <source>!File system not recognized:
No Files</source>
        <translation>Sistema de archivos no reconocido:
No hay archivos</translation>
    </message>
    <message>
        <location filename="../src/diskbrowser/diskbrowserdlg.cpp" line="281"/>
        <source>!SIO device not available:
No Files</source>
        <translation>Dispositivo SIO no disponible:
No hay archivos</translation>
    </message>
</context>
<context>
    <name>DiskEditDialog</name>
    <message>
        <location filename="../ui/diskeditdialog.ui" line="23"/>
        <source>MainWindow</source>
        <translation>Ventana Principal</translation>
    </message>
    <message>
        <location filename="../ui/diskeditdialog.ui" line="92"/>
        <source>Stay on Top</source>
        <translation>Permanecer encima</translation>
    </message>
    <message>
        <location filename="../ui/diskeditdialog.ui" line="111"/>
        <source>toolBar</source>
        <translation>Barra de Herramientas</translation>
    </message>
    <message>
        <location filename="../ui/diskeditdialog.ui" line="150"/>
        <location filename="../ui/diskeditdialog.ui" line="153"/>
        <location filename="../ui/diskeditdialog.ui" line="156"/>
        <source>Go to the parent directory</source>
        <translation>Vaya al directorio padre</translation>
    </message>
    <message>
        <location filename="../ui/diskeditdialog.ui" line="165"/>
        <source>Add files...</source>
        <translation>Agregar archivos...</translation>
    </message>
    <message>
        <location filename="../ui/diskeditdialog.ui" line="168"/>
        <location filename="../ui/diskeditdialog.ui" line="171"/>
        <source>Add files to this directory</source>
        <translation>Agregar archivos a este directorio</translation>
    </message>
    <message>
        <location filename="../ui/diskeditdialog.ui" line="183"/>
        <source>Extract files...</source>
        <translation>Extraer archivos...</translation>
    </message>
    <message>
        <location filename="../ui/diskeditdialog.ui" line="186"/>
        <location filename="../ui/diskeditdialog.ui" line="189"/>
        <source>Extract selected files</source>
        <translation>Extrae los archivos seleccionados</translation>
    </message>
    <message>
        <location filename="../ui/diskeditdialog.ui" line="201"/>
        <source>Text conversion</source>
        <translation>Conversión de Texto</translation>
    </message>
    <message>
        <location filename="../ui/diskeditdialog.ui" line="204"/>
        <location filename="../ui/diskeditdialog.ui" line="207"/>
        <location filename="../src/diskeditdialog.cpp" line="614"/>
        <location filename="../src/diskeditdialog.cpp" line="615"/>
        <source>Text conversion is off</source>
        <translation>Conversión de texto desactivado</translation>
    </message>
    <message>
        <location filename="../ui/diskeditdialog.ui" line="219"/>
        <source>Delete</source>
        <translation>Borrar</translation>
    </message>
    <message>
        <location filename="../ui/diskeditdialog.ui" line="222"/>
        <location filename="../ui/diskeditdialog.ui" line="225"/>
        <source>Delete selected files</source>
        <translation>Eliminar archivos seleccionados</translation>
    </message>
    <message>
        <location filename="../ui/diskeditdialog.ui" line="228"/>
        <source>Del</source>
        <translation>Eliminar</translation>
    </message>
    <message>
        <location filename="../ui/diskeditdialog.ui" line="237"/>
        <source>Print</source>
        <translation>Imprimir</translation>
    </message>
    <message>
        <location filename="../ui/diskeditdialog.ui" line="240"/>
        <source>Print Directory Listing</source>
        <translation>Imprimir lista de directorio</translation>
    </message>
    <message>
        <location filename="../src/diskeditdialog.cpp" line="448"/>
        <source>No file system</source>
        <translation>No existe un sistema de archivos</translation>
    </message>
    <message>
        <location filename="../src/diskeditdialog.cpp" line="449"/>
        <source>Atari Dos 1.0</source>
        <translation></translation>
    </message>
    <message>
        <location filename="../src/diskeditdialog.cpp" line="450"/>
        <source>Atari Dos 2.0</source>
        <translation></translation>
    </message>
    <message>
        <location filename="../src/diskeditdialog.cpp" line="451"/>
        <source>Atari Dos 2.5</source>
        <translation></translation>
    </message>
    <message>
        <location filename="../src/diskeditdialog.cpp" line="452"/>
        <source>MyDos</source>
        <translation></translation>
    </message>
    <message>
        <location filename="../src/diskeditdialog.cpp" line="453"/>
        <source>SpartaDos</source>
        <translation></translation>
    </message>
    <message>
        <location filename="../src/diskeditdialog.cpp" line="549"/>
        <location filename="../src/diskeditdialog.cpp" line="568"/>
        <location filename="../src/diskeditdialog.cpp" line="581"/>
        <source>RespeQt - Exploring %1</source>
        <translation>RespeQt - Exploración %1</translation>
    </message>
    <message>
        <location filename="../src/diskeditdialog.cpp" line="591"/>
        <source>Extract files</source>
        <translation>Extraer archivos</translation>
    </message>
    <message>
        <location filename="../src/diskeditdialog.cpp" line="610"/>
        <location filename="../src/diskeditdialog.cpp" line="611"/>
        <source>Text conversion is on</source>
        <translation>Conversión de texto activado</translation>
    </message>
    <message>
        <location filename="../src/diskeditdialog.cpp" line="621"/>
        <source>Confirmation</source>
        <translation>Confirmación</translation>
    </message>
    <message>
        <location filename="../src/diskeditdialog.cpp" line="621"/>
        <source>Are you sure you want to delete selected files?</source>
        <translation>¿Está seguro que desea eliminar los archivos seleccionados?</translation>
    </message>
    <message>
        <location filename="../src/diskeditdialog.cpp" line="630"/>
        <source>Add files</source>
        <translation>Agregar archivos</translation>
    </message>
</context>
<context>
    <name>DiskImages::DiskGeometry</name>
    <message>
        <location filename="../src/diskimages/diskgeometry.cpp" line="199"/>
        <source>SD Diskette</source>
        <translation>Diskette SD</translation>
    </message>
    <message>
        <location filename="../src/diskimages/diskgeometry.cpp" line="201"/>
        <source>ED Diskette</source>
        <translation>Diskette ED</translation>
    </message>
    <message>
        <location filename="../src/diskimages/diskgeometry.cpp" line="203"/>
        <source>DD Diskette</source>
        <translation>Diskette DD</translation>
    </message>
    <message>
        <location filename="../src/diskimages/diskgeometry.cpp" line="205"/>
        <location filename="../src/diskimages/diskgeometry.cpp" line="207"/>
        <source>DS/DD Diskette</source>
        <translation>Diskette DS/DD</translation>
    </message>
    <message>
        <location filename="../src/diskimages/diskgeometry.cpp" line="210"/>
        <source>%1 sector SD hard disk</source>
        <translation>%1 sector SD disco duro</translation>
    </message>
    <message>
        <location filename="../src/diskimages/diskgeometry.cpp" line="212"/>
        <source>%1 sector DD hard disk</source>
        <translation>%1 sector DD disco duro</translation>
    </message>
    <message>
        <location filename="../src/diskimages/diskgeometry.cpp" line="214"/>
        <source>%1 sector, %2 bytes/sector hard disk</source>
        <translation>%1 sector, %2 bytes/sector del disco duro</translation>
    </message>
    <message>
        <location filename="../src/diskimages/diskgeometry.cpp" line="217"/>
        <source>%1 %2 tracks/side, %3 sectors/track, %4 bytes/sector diskette</source>
        <translation>%1 %2 tracks/side, %3 sectors/track, %4 bytes/sector disco</translation>
    </message>
    <message>
        <location filename="../src/diskimages/diskgeometry.cpp" line="218"/>
        <source>DS</source>
        <translation></translation>
    </message>
    <message>
        <location filename="../src/diskimages/diskgeometry.cpp" line="218"/>
        <source>SS</source>
        <translation></translation>
    </message>
    <message>
        <location filename="../src/diskimages/diskgeometry.cpp" line="224"/>
        <source>%1 (%2k)</source>
        <translation></translation>
    </message>
</context>
<context>
    <name>DiskImages::FolderImage</name>
    <message>
        <location filename="../include/diskimages/folderimage.h" line="56"/>
        <source>Folder image</source>
        <translation>Imagen de carpeta</translation>
    </message>
    <message>
        <location filename="../src/diskimages/folderimage.cpp" line="136"/>
        <source>Cannot mirror %1 of %2 files in &apos;%3&apos;: Atari directory is full.</source>
        <translation>No se puede reflejar %1 de %2 archivos en &apos;%3&apos;: Directorio está lleno.</translation>
    </message>
</context>
<context>
    <name>DiskImages::SimpleDiskImage</name>
    <message>
        <location filename="../src/diskimages/diskimage.cpp" line="75"/>
        <location filename="../src/diskimages/diskimage.cpp" line="77"/>
        <location filename="../src/diskimages/diskimage.cpp" line="79"/>
        <source>Favorite tool disk</source>
        <translation>Disco de herramientas favorito</translation>
    </message>
    <message>
        <location filename="../src/diskimages/diskimage.cpp" line="75"/>
        <location filename="../src/diskimages/diskimage.cpp" line="87"/>
        <source>CHIP mode</source>
        <translation>Modo CHIP</translation>
    </message>
    <message>
        <location filename="../src/diskimages/diskimage.cpp" line="77"/>
        <location filename="../src/diskimages/diskimage.cpp" line="89"/>
        <source>HAPPY mode</source>
        <translation>Modo HAPPY</translation>
    </message>
    <message>
        <location filename="../src/diskimages/diskimage.cpp" line="84"/>
        <source>Image %1/%2</source>
        <translation>Imagen %1/%2</translation>
    </message>
    <message>
        <location filename="../src/diskimages/diskimage.cpp" line="100"/>
        <source>Load image %1 of %2:
%3</source>
        <translation>Cargar imagen %1 de %2:
%3</translation>
    </message>
    <message>
        <location filename="../src/diskimages/diskimage.cpp" line="166"/>
        <source>[%1] No Translator disk image defined. Please, check settings in menu Disk images&gt;OS-B emulation.</source>
        <translation>[%1] No se ha definido ninguna imagen de disco de traductor. Por favor, verifique la configuración en el menú Imágenes de disco Emulación OS-B.</translation>
    </message>
    <message>
        <location filename="../src/diskimages/diskimage.cpp" line="172"/>
        <source>[%1] Translator &apos;%2&apos; not found. Please, check settings in menu Disk images&gt;OS-B emulation.</source>
        <translation>[%1] Traductor &apos;%2&apos; extraviado. Por favor, compruebe la configuración en el menú Imágenes de disco Emulación OS-B.</translation>
    </message>
    <message>
        <location filename="../src/diskimages/diskimage.cpp" line="205"/>
        <source>[%1] No tool disk image defined. Please, check settings in menu Disk images&gt;Favorite tool disk.</source>
        <translation>[%1] No se ha definido ninguna imagen de disco de herramientas. Por favor, compruebe la configuración en el menú Imágenes de disco Disco de herramientas favorito.</translation>
    </message>
    <message>
        <location filename="../src/diskimages/diskimage.cpp" line="211"/>
        <source>[%1] Tool disk &apos;%2&apos; not found. Please, check settings in menu Disk images&gt;Favorite tool disk.</source>
        <translation>[%1] Disco de herramientas &apos;%2&apos; extraviado. Por favor, verifique la configuración en el menú Imágenes de disco Disco de herramientas favorito.</translation>
    </message>
    <message>
        <location filename="../src/diskimages/diskimage.cpp" line="261"/>
        <source>[%1] Drive door lever open. Drive is no longer ready</source>
        <translation>[%1] Palanca de la puerta de accionamiento abierta. Drive ya no está listo</translation>
    </message>
    <message>
        <location filename="../src/diskimages/diskimage.cpp" line="263"/>
        <source>[%1] Drive door lever closed. Drive is now ready</source>
        <translation>[%1] Palanca de la puerta de transmisión cerrada. Drive ya está listo</translation>
    </message>
    <message>
        <location filename="../src/diskimages/diskimage.cpp" line="269"/>
        <location filename="../src/diskimages/diskimage.cpp" line="274"/>
        <location filename="../src/diskimages/diskimage.cpp" line="310"/>
        <location filename="../src/diskimages/diskimageatr.cpp" line="39"/>
        <location filename="../src/diskimages/diskimageatr.cpp" line="48"/>
        <location filename="../src/diskimages/diskimageatr.cpp" line="57"/>
        <location filename="../src/diskimages/diskimageatr.cpp" line="72"/>
        <location filename="../src/diskimages/diskimageatr.cpp" line="83"/>
        <location filename="../src/diskimages/diskimageatr.cpp" line="90"/>
        <location filename="../src/diskimages/diskimageatr.cpp" line="110"/>
        <location filename="../src/diskimages/diskimageatr.cpp" line="156"/>
        <location filename="../src/diskimages/diskimageatr.cpp" line="167"/>
        <location filename="../src/diskimages/diskimageatr.cpp" line="180"/>
        <location filename="../src/diskimages/diskimageatr.cpp" line="213"/>
        <location filename="../src/diskimages/diskimageatr.cpp" line="220"/>
        <location filename="../src/diskimages/diskimageatr.cpp" line="230"/>
        <location filename="../src/diskimages/diskimageatr.cpp" line="237"/>
        <location filename="../src/diskimages/diskimageatr.cpp" line="248"/>
        <location filename="../src/diskimages/diskimageatx.cpp" line="31"/>
        <location filename="../src/diskimages/diskimageatx.cpp" line="40"/>
        <location filename="../src/diskimages/diskimageatx.cpp" line="49"/>
        <location filename="../src/diskimages/diskimagepro.cpp" line="38"/>
        <location filename="../src/diskimages/diskimagepro.cpp" line="47"/>
        <location filename="../src/diskimages/diskimagepro.cpp" line="55"/>
        <location filename="../src/diskimages/diskimagepro.cpp" line="61"/>
        <location filename="../src/diskimages/diskimagepro.cpp" line="70"/>
        <location filename="../src/diskimages/diskimagepro.cpp" line="77"/>
        <location filename="../src/diskimages/diskimagepro.cpp" line="2223"/>
        <location filename="../src/diskimages/diskimagepro.cpp" line="2254"/>
        <source>Cannot open &apos;%1&apos;: %2</source>
        <translation>No se puede abrir &apos;%1&apos;: %2</translation>
    </message>
    <message>
        <location filename="../src/diskimages/diskimage.cpp" line="269"/>
        <source>DCM images are not supported yet.</source>
        <translation>Las imágenes de DCM aún no son compatibles.</translation>
    </message>
    <message>
        <location filename="../src/diskimages/diskimage.cpp" line="274"/>
        <source>DI images are not supported yet.</source>
        <translation>Las imágenes DI aún no son compatibles.</translation>
    </message>
    <message>
        <location filename="../src/diskimages/diskimage.cpp" line="310"/>
        <location filename="../src/diskimages/diskimage.cpp" line="441"/>
        <source>Unknown file type.</source>
        <translation>Tipo de archivo desconocido.</translation>
    </message>
    <message>
        <location filename="../src/diskimages/diskimage.cpp" line="317"/>
        <source>Translator &apos;%1&apos; activated</source>
        <translation>Traductor &apos;%1&apos; activado</translation>
    </message>
    <message>
        <location filename="../src/diskimages/diskimage.cpp" line="356"/>
        <source>Image is %1 of %2. Next image will be %3</source>
        <translation>La imagen es %1 de %2. La siguiente imagen será %3</translation>
    </message>
    <message>
        <location filename="../src/diskimages/diskimage.cpp" line="405"/>
        <location filename="../src/diskimages/diskimage.cpp" line="410"/>
        <location filename="../src/diskimages/diskimage.cpp" line="441"/>
        <location filename="../src/diskimages/diskimage.cpp" line="521"/>
        <location filename="../src/diskimages/diskimageatr.cpp" line="297"/>
        <location filename="../src/diskimages/diskimageatr.cpp" line="305"/>
        <location filename="../src/diskimages/diskimageatr.cpp" line="313"/>
        <location filename="../src/diskimages/diskimageatr.cpp" line="326"/>
        <location filename="../src/diskimages/diskimageatr.cpp" line="332"/>
        <location filename="../src/diskimages/diskimageatr.cpp" line="379"/>
        <location filename="../src/diskimages/diskimageatr.cpp" line="385"/>
        <location filename="../src/diskimages/diskimageatr.cpp" line="397"/>
        <location filename="../src/diskimages/diskimageatr.cpp" line="402"/>
        <location filename="../src/diskimages/diskimageatr.cpp" line="422"/>
        <location filename="../src/diskimages/diskimageatr.cpp" line="452"/>
        <location filename="../src/diskimages/diskimageatr.cpp" line="459"/>
        <location filename="../src/diskimages/diskimageatr.cpp" line="481"/>
        <location filename="../src/diskimages/diskimageatx.cpp" line="264"/>
        <location filename="../src/diskimages/diskimageatx.cpp" line="272"/>
        <location filename="../src/diskimages/diskimageatx.cpp" line="302"/>
        <location filename="../src/diskimages/diskimageatx.cpp" line="329"/>
        <location filename="../src/diskimages/diskimageatx.cpp" line="339"/>
        <location filename="../src/diskimages/diskimageatx.cpp" line="351"/>
        <location filename="../src/diskimages/diskimageatx.cpp" line="370"/>
        <location filename="../src/diskimages/diskimageatx.cpp" line="382"/>
        <location filename="../src/diskimages/diskimageatx.cpp" line="395"/>
        <location filename="../src/diskimages/diskimageatx.cpp" line="415"/>
        <location filename="../src/diskimages/diskimagepro.cpp" line="211"/>
        <location filename="../src/diskimages/diskimagepro.cpp" line="218"/>
        <location filename="../src/diskimages/diskimagepro.cpp" line="240"/>
        <location filename="../src/diskimages/diskimagepro.cpp" line="250"/>
        <location filename="../src/diskimages/diskimagepro.cpp" line="270"/>
        <location filename="../src/diskimages/diskimagepro.cpp" line="280"/>
        <location filename="../src/diskimages/diskimagepro.cpp" line="302"/>
        <source>Cannot save &apos;%1&apos;: %2</source>
        <translation>No se puede guardar &apos;%1&apos;:%2</translation>
    </message>
    <message>
        <location filename="../src/diskimages/diskimage.cpp" line="405"/>
        <source>Saving DCM images is not supported yet.</source>
        <translation>Aún no se admite guardar imágenes de DCM.</translation>
    </message>
    <message>
        <location filename="../src/diskimages/diskimage.cpp" line="410"/>
        <source>Saving DI images is not supported yet.</source>
        <translation>Aún no se admite guardar imágenes DI.</translation>
    </message>
    <message>
        <location filename="../src/diskimages/diskimage.cpp" line="521"/>
        <source>Unknown file extension.</source>
        <translation>Extensión de archivo desconocida.</translation>
    </message>
    <message>
        <location filename="../src/diskimages/diskimage.cpp" line="564"/>
        <source>[%1] Uploaded code is: Check if drive is a Super Archiver</source>
        <translation>[%1] El código subido es: compruebe si la unidad es un superarchivador</translation>
    </message>
    <message>
        <location filename="../src/diskimages/diskimage.cpp" line="583"/>
        <source>[%1] Uploaded code is: Speed check</source>
        <translation>[%1] El código subido es: verificación de velocidad</translation>
    </message>
    <message>
        <location filename="../src/diskimages/diskimage.cpp" line="585"/>
        <source>[%1] Uploaded code is: Diagnostic</source>
        <translation>[%1] El código subido es: diagnóstico</translation>
    </message>
    <message>
        <location filename="../src/diskimages/diskimage.cpp" line="587"/>
        <source>[%1] Uploaded code is: End of diagnostic</source>
        <translation>[%1] El código subido es: fin del diagnóstico</translation>
    </message>
    <message>
        <location filename="../src/diskimages/diskimage.cpp" line="589"/>
        <source>[%1] Uploaded code is: Read address marks for track $%2</source>
        <translation>[%1] El código subido es: leer las marcas de dirección de la pista $%2</translation>
    </message>
    <message>
        <location filename="../src/diskimages/diskimage.cpp" line="591"/>
        <source>[%1] Uploaded code is: BitWriter clear memory</source>
        <translation>[%1] El código cargado es: BitWriter borrar memoria</translation>
    </message>
    <message>
        <location filename="../src/diskimages/diskimage.cpp" line="593"/>
        <source>[%1] Uploaded code is: BitWriter read track $%2</source>
        <translation>[%1] El código cargado es: BitWriter leyó la pista $%2</translation>
    </message>
    <message>
        <location filename="../src/diskimages/diskimage.cpp" line="599"/>
        <source>[%1] Uploaded code is: Prepare track data at offset $%2</source>
        <translation>[%1] El código subido es: preparar los datos de la pista en el desplazamiento $%2</translation>
    </message>
    <message>
        <location filename="../src/diskimages/diskimage.cpp" line="601"/>
        <source>[%1] Uploaded code is: Super Archiver open Chip</source>
        <translation>[%1] El código subido es: chip abierto de Super Archiver</translation>
    </message>
    <message>
        <location filename="../src/diskimages/diskimage.cpp" line="603"/>
        <source>[%1] Uploaded code is: BitWriter open Chip</source>
        <translation>[%1] El código cargado es: BitWriter open Chip</translation>
    </message>
    <message>
        <location filename="../src/diskimages/diskimage.cpp" line="605"/>
        <source>[%1] Uploaded code is: Super Archiver clear memory</source>
        <translation>[%1] El código subido es: Super Archiver borrar memoria</translation>
    </message>
    <message>
        <location filename="../src/diskimages/diskimage.cpp" line="619"/>
        <source>[%1] Uploaded code is: Skew alignment of track $%2 sector $%3 with track $%4 sector $%5</source>
        <translation>[%1] El código cargado es: Alineación sesgada de la pista $%2 sector $%3 con la pista $%4 sector $%5</translation>
    </message>
    <message>
        <location filename="../src/diskimages/diskimage.cpp" line="621"/>
        <source>[%1] Uploaded code is: Skew alignment of track $%2 (%3 sectors) with track $%4 (%5 sectors)</source>
        <translation>[%1] El código cargado es: Alineación sesgada de la pista $%2 (%3 sectores) con la pista $%4 (%5 sectores)</translation>
    </message>
    <message>
        <location filename="../src/diskimages/diskimage.cpp" line="628"/>
        <source>[%1] Uploaded code is: Format track $%2 with skew alignment $%3 with track $%4</source>
        <translation>[%1] El código subido es: Formatee la pista $%2 con alineación sesgada $%3 con la pista $%4</translation>
    </message>
    <message>
        <location filename="../src/diskimages/diskimage.cpp" line="630"/>
        <source>[%1] Uploaded code is: Format track $%2 with skew alignment with track $%3</source>
        <translation>[%1] El código subido es: Formatee la pista $%2 con alineación sesgada con la pista $%3</translation>
    </message>
    <message>
        <location filename="../src/diskimages/diskimage.cpp" line="634"/>
        <source>[%1] Data Crc16 is $%2. Command ignored</source>
        <translation>[%1] Los datos Crc16 son $%2. Comando ignorado</translation>
    </message>
    <message>
        <location filename="../src/diskimages/diskimage.cpp" line="982"/>
        <source>[%1] command: $%2, aux: $%3 ignored because the drive is not ready.</source>
        <translation>[%1] comando: $%2, aux: $%3 ignorado porque la unidad no está lista.</translation>
    </message>
    <message>
        <location filename="../src/diskimages/diskimage.cpp" line="1010"/>
        <source>[%1] Booting Translator &apos;%2&apos; first</source>
        <translation>[%1] Iniciando Traductor &apos;%2&apos; primero</translation>
    </message>
    <message>
        <location filename="../src/diskimages/diskimage.cpp" line="1016"/>
        <source>[%1] Removing Translator to boot on &apos;%2&apos;</source>
        <translation>[%1] Eliminando el Traductor para arrancar en &apos;%2&apos;</translation>
    </message>
    <message>
        <location filename="../src/diskimages/diskimage.cpp" line="1047"/>
        <source>[%1] Booting tool disk &apos;%2&apos; first</source>
        <translation>[%1] Disco de la herramienta de arranque &apos;%2&apos; primero</translation>
    </message>
    <message>
        <location filename="../src/diskimages/diskimage.cpp" line="1067"/>
        <source>[%1] Format.</source>
        <translation>[%1] Formato.</translation>
    </message>
    <message>
        <location filename="../src/diskimages/diskimage.cpp" line="1071"/>
        <source>[%1] Format failed.</source>
        <translation>[%1] Error de formato.</translation>
    </message>
    <message>
        <location filename="../src/diskimages/diskimage.cpp" line="1074"/>
        <source>[%1] Format denied.</source>
        <translation>[%1] Formato denegado.</translation>
    </message>
    <message>
        <location filename="../src/diskimages/diskimage.cpp" line="1089"/>
        <source>[%1] Format ED denied.</source>
        <translation>[%1] Formato ED denegado.</translation>
    </message>
    <message>
        <location filename="../src/diskimages/diskimage.cpp" line="1094"/>
        <source>[%1] Format ED.</source>
        <translation>[%1] Formatear ED.</translation>
    </message>
    <message>
        <location filename="../src/diskimages/diskimage.cpp" line="1098"/>
        <source>[%1] Format ED failed.</source>
        <translation>[%1] Error en el formato ED.</translation>
    </message>
    <message>
        <location filename="../src/diskimages/diskimage.cpp" line="1109"/>
        <source>[%1] Run Speed Diagnostic with AUX1=$%2 and AUX2=$%3</source>
        <translation>[%1] Ejecutar diagnóstico de velocidad con AUX1=$%2 y AUX2=$%3</translation>
    </message>
    <message>
        <location filename="../src/diskimages/diskimage.cpp" line="1124"/>
        <source>[%1] Run Diagnostic with AUX1=$%2 and AUX2=$%3</source>
        <translation>[%1] Ejecutar diagnóstico con AUX1=$%2 y AUX2=$%3</translation>
    </message>
    <message>
        <location filename="../src/diskimages/diskimage.cpp" line="1161"/>
        <source>[%1] Speed poll: %2</source>
        <translation>[%1] Encuesta rápida: %2</translation>
    </message>
    <message>
        <location filename="../src/diskimages/diskimage.cpp" line="1175"/>
        <location filename="../src/diskimages/diskimage.cpp" line="1819"/>
        <source>[%1] Happy Read Track %2 ($%3)</source>
        <translation>[%1] Pista de lectura Happy %2 ($%3)</translation>
    </message>
    <message>
        <location filename="../src/diskimages/diskimage.cpp" line="1182"/>
        <location filename="../src/diskimages/diskimage.cpp" line="1371"/>
        <location filename="../src/diskimages/diskimage.cpp" line="1520"/>
        <location filename="../src/diskimages/diskimage.cpp" line="1835"/>
        <location filename="../src/diskimages/diskimage.cpp" line="1846"/>
        <location filename="../src/diskimages/diskimage.cpp" line="2178"/>
        <source>[%1] Happy Execute custom code $%2 with AUX1 $%3, AUX2 $%4 and CRC16 $%5. Ignored</source>
        <translation></translation>
    </message>
    <message>
        <location filename="../src/diskimages/diskimage.cpp" line="1186"/>
        <source>[%1] Command $41 NAKed (HAPPY is not enabled).</source>
        <translation>[%1] Comando $41 NAKed (HAPPY no está habilitado).</translation>
    </message>
    <message>
        <location filename="../src/diskimages/diskimage.cpp" line="1202"/>
        <location filename="../src/diskimages/diskimage.cpp" line="2131"/>
        <source>[%1] Happy Read Sectors of track %2 ($%3)</source>
        <translation>[%1] Sectores de lectura Happy de la pista %2 ($%3)</translation>
    </message>
    <message>
        <location filename="../src/diskimages/diskimage.cpp" line="1204"/>
        <location filename="../src/diskimages/diskimage.cpp" line="2133"/>
        <source>[%1] Happy Read Sectors of track %2 ($%3) starting after sector %4 ($%5)</source>
        <translation>[%1] Sectores de lectura feliz de la pista %2 ($%3) a partir del sector %4 ($%5)</translation>
    </message>
    <message>
        <location filename="../src/diskimages/diskimage.cpp" line="1214"/>
        <location filename="../src/diskimages/diskimage.cpp" line="1268"/>
        <location filename="../src/diskimages/diskimage.cpp" line="1323"/>
        <location filename="../src/diskimages/diskimage.cpp" line="1437"/>
        <source>[%1] Happy Execute custom code $%2 with AUX $%3 and CRC16 $%4. Ignored</source>
        <translation>[%1] Happy Execute código personalizado $%2 con AUX $%3 y CRC16 $%4. Ignorado</translation>
    </message>
    <message>
        <location filename="../src/diskimages/diskimage.cpp" line="1220"/>
        <source>[%1] Super Archiver Write Sector using Index denied (CHIP is not open)</source>
        <translation>[%1] Sector de escritura de Super Archiver que usa índice denegado (CHIP no está abierto)</translation>
    </message>
    <message>
        <location filename="../src/diskimages/diskimage.cpp" line="1227"/>
        <source>[%1] Super Archiver Write Sector using Index with AUX1=$%2 and AUX2=$%3</source>
        <translation>[%1] Sector de escritura del súper archivador usando índice con AUX1=$%2 y AUX2=$%3</translation>
    </message>
    <message>
        <location filename="../src/diskimages/diskimage.cpp" line="1235"/>
        <source>[%1] Super Archiver Write Sector using Index denied.</source>
        <translation>[%1] Sector de escritura de Super Archiver que usa índice denegado.</translation>
    </message>
    <message>
        <location filename="../src/diskimages/diskimage.cpp" line="1242"/>
        <source>[%1] Super Archiver Write Sector using Index failed.</source>
        <translation>[%1] Error en el sector de escritura del Super Archiver al utilizar el índice.</translation>
    </message>
    <message>
        <location filename="../src/diskimages/diskimage.cpp" line="1245"/>
        <source>[%1] Super Archiver Write Sector using Index data frame failed.</source>
        <translation>[%1] Error en el sector de escritura del Super Archiver utilizando el marco de datos de índice.</translation>
    </message>
    <message>
        <location filename="../src/diskimages/diskimage.cpp" line="1263"/>
        <source>[%1] Happy Set Skew Alignment track %2 ($%3) and sector %4 ($%5)</source>
        <translation>[%1] Happy Establecer pista de alineación sesgada %2 ($%3) y sector %4 ($%5)</translation>
    </message>
    <message>
        <location filename="../src/diskimages/diskimage.cpp" line="1274"/>
        <source>[%1] Super Archiver Read all Sector Statuses denied (CHIP is not open)</source>
        <translation>[%1] Super Archiver Lee todos los estados de sector denegados (CHIP no está abierto)</translation>
    </message>
    <message>
        <location filename="../src/diskimages/diskimage.cpp" line="1281"/>
        <source>[%1] Super Archiver Read All Sector Statuses</source>
        <translation>[%1] Super Archiver lee todos los estados del sector</translation>
    </message>
    <message>
        <location filename="../src/diskimages/diskimage.cpp" line="1289"/>
        <source>[%1] Super Archiver Read All Sector Statuses failed.</source>
        <translation>[%1] Error en la lectura de todos los estados del sector de Super Archiver.</translation>
    </message>
    <message>
        <location filename="../src/diskimages/diskimage.cpp" line="1308"/>
        <location filename="../src/diskimages/diskimage.cpp" line="2115"/>
        <source>[%1] Happy Read Skew alignment of track %2 ($%3) sector %4 ($%5) with track %6 ($%7) sector %8 ($%9)</source>
        <translation>[%1] Lectura Happy Alineación sesgada de la pista %2 ($%3) sector %4 ($%5) con la pista %6 ($%7) sector %8 ($%9)</translation>
    </message>
    <message>
        <location filename="../src/diskimages/diskimage.cpp" line="1316"/>
        <location filename="../src/diskimages/diskimage.cpp" line="2121"/>
        <source>[%1] Happy Read Skew alignment failed.</source>
        <translation>[%1] Error en la alineación del sesgo de lectura Happy.</translation>
    </message>
    <message>
        <location filename="../src/diskimages/diskimage.cpp" line="1329"/>
        <source>[%1] Super Archiver Read Sector using Index denied (CHIP is not open)</source>
        <translation>[%1] Sector de lectura de Super Archiver que usa índice denegado (CHIP no está abierto)</translation>
    </message>
    <message>
        <location filename="../src/diskimages/diskimage.cpp" line="1336"/>
        <source>[%1] Super Archiver Read Sector using Index with AUX1=$%2 and AUX2=$%3</source>
        <translation>[%1] Sector de lectura de Super Archiver usando índice con AUX1=$%2 y AUX2=$%3</translation>
    </message>
    <message>
        <location filename="../src/diskimages/diskimage.cpp" line="1344"/>
        <source>[%1] Super Archiver Read sector using Index with AUX1=$%2 and AUX2=$%3 failed.</source>
        <translation>[%1] Error en el sector de lectura de Super Archiver que usaba el índice con AUX1=$%2 y AUX2=$%3.</translation>
    </message>
    <message>
        <location filename="../src/diskimages/diskimage.cpp" line="1361"/>
        <location filename="../src/diskimages/diskimage.cpp" line="2036"/>
        <source>[%1] Happy Write track %2 ($%3)</source>
        <translation>[%1] Happy Escribir track %2 ($%3)</translation>
    </message>
    <message>
        <location filename="../src/diskimages/diskimage.cpp" line="1368"/>
        <location filename="../src/diskimages/diskimage.cpp" line="1495"/>
        <location filename="../src/diskimages/diskimage.cpp" line="2041"/>
        <location filename="../src/diskimages/diskimage.cpp" line="2167"/>
        <source>[%1] Happy Write track failed.</source>
        <translation>[%1] Error en la pista de escritura Happy.</translation>
    </message>
    <message>
        <location filename="../src/diskimages/diskimage.cpp" line="1377"/>
        <source>[%1] Super Archiver Write Track denied (CHIP is not open)</source>
        <translation>[%1] Seguimiento de escritura de Super Archiver denegado (CHIP no está abierto)</translation>
    </message>
    <message>
        <location filename="../src/diskimages/diskimage.cpp" line="1384"/>
        <source>[%1] Super Archiver Write Track with AUX1=$%2 and AUX2=$%3</source>
        <translation>[%1] Pista de escritura de Super Archiver con AUX1=$%2 y AUX2=$%3</translation>
    </message>
    <message>
        <location filename="../src/diskimages/diskimage.cpp" line="1393"/>
        <source>[%1] Super Archiver Write Track denied.</source>
        <translation>[%1] Se rechazó la pista de escritura de Super Archiver.</translation>
    </message>
    <message>
        <location filename="../src/diskimages/diskimage.cpp" line="1400"/>
        <source>[%1] Super Archiver Write track failed.</source>
        <translation>[%1] Error en la pista de escritura de Super Archiver.</translation>
    </message>
    <message>
        <location filename="../src/diskimages/diskimage.cpp" line="1403"/>
        <source>[%1] Super Archiver Write track data frame failed.</source>
        <translation>[%1] Error en el marco de datos de la pista de escritura de Super Archiver.</translation>
    </message>
    <message>
        <location filename="../src/diskimages/diskimage.cpp" line="1419"/>
        <location filename="../src/diskimages/diskimage.cpp" line="2144"/>
        <source>[%1] Happy Write Sectors of track %2 ($%3)</source>
        <translation>[%1] Sectores de escritura Happy de la pista %2 ($%3)</translation>
    </message>
    <message>
        <location filename="../src/diskimages/diskimage.cpp" line="1421"/>
        <location filename="../src/diskimages/diskimage.cpp" line="2146"/>
        <source>[%1] Happy Write Sectors of track %2 ($%3) starting after sector %4 ($%5)</source>
        <translation>[%1] Happy Write Sectores de la pista %2 ($%3) que comienzan después del sector %4 ($%5)</translation>
    </message>
    <message>
        <location filename="../src/diskimages/diskimage.cpp" line="1430"/>
        <location filename="../src/diskimages/diskimage.cpp" line="2153"/>
        <source>[%1] Happy Write Sectors failed.</source>
        <translation>[%1] Falló el sector de escritura Happy.</translation>
    </message>
    <message>
        <location filename="../src/diskimages/diskimage.cpp" line="1443"/>
        <source>[%1] Super Archiver Read Track (128 bytes) denied (CHIP is not open)</source>
        <translation>[%1] Super Archiver Read Track (128 bytes) denegado (CHIP no está abierto)</translation>
    </message>
    <message>
        <location filename="../src/diskimages/diskimage.cpp" line="1450"/>
        <source>[%1] Super Archiver Read Track (128 bytes) with AUX1=$%2 and AUX2=$%3</source>
        <translation>[%1] Pista de lectura del superarchivador (128 bytes) con AUX1=$%2 y AUX2=$%3</translation>
    </message>
    <message>
        <location filename="../src/diskimages/diskimage.cpp" line="1466"/>
        <source>[%1] Happy Configure drive with AUX1 $%2, AUX2 $%3</source>
        <translation></translation>
    </message>
    <message>
        <location filename="../src/diskimages/diskimage.cpp" line="1476"/>
        <source>[%1] Happy Configure drive NAKed (HAPPY is not enabled).</source>
        <translation>[%1] Unidad Happy configurada NAKed (HAPPY no está habilitado).</translation>
    </message>
    <message>
        <location filename="../src/diskimages/diskimage.cpp" line="1488"/>
        <source>[%1] Happy Write track using skew alignment of track %2 ($%3) with track %4 ($%5) sector %6 ($%7)</source>
        <translation>[%1] Pista de escritura Happy usando alineación sesgada de la pista %2 ($%3) con la pista %4 ($%5) sector %6 ($%7)</translation>
    </message>
    <message>
        <location filename="../src/diskimages/diskimage.cpp" line="1499"/>
        <source>[%1] Happy Execute custom code $%2 with AUX1 $%3, $%4 and CRC16 $%5. Ignored</source>
        <translation></translation>
    </message>
    <message>
        <location filename="../src/diskimages/diskimage.cpp" line="1503"/>
        <source>[%1] Command $49 NAKed (HAPPY is not enabled).</source>
        <translation>[%1] Comando $49 NAKed (HAPPY no está habilitado).</translation>
    </message>
    <message>
        <location filename="../src/diskimages/diskimage.cpp" line="1515"/>
        <source>[%1] Happy Init Skew alignment</source>
        <translation>[%1] Alineación sesgada inicial Happy</translation>
    </message>
    <message>
        <location filename="../src/diskimages/diskimage.cpp" line="1524"/>
        <source>[%1] Command $4A NAKed (HAPPY is not enabled).</source>
        <translation>[%1] Comando $4A NAKed (HAPPY no está habilitado).</translation>
    </message>
    <message>
        <location filename="../src/diskimages/diskimage.cpp" line="1535"/>
        <source>[%1] Happy Prepare backup with AUX1 $%2, AUX2 $%3</source>
        <translation>[%1] Happy Preparar copia de seguridad con AUX1 $%2, AUX2 $%3</translation>
    </message>
    <message>
        <location filename="../src/diskimages/diskimage.cpp" line="1542"/>
        <source>[%1] Happy Prepare backup NAKed (HAPPY is not enabled).</source>
        <translation>[%1] Happy Prepare copia de seguridad NAKed (HAPPY no está habilitado).</translation>
    </message>
    <message>
        <location filename="../src/diskimages/diskimage.cpp" line="1549"/>
        <source>[%1] Super Archiver Set RAM Buffer denied (CHIP is not open)</source>
        <translation>[%1] Super Archiver establecido buffer RAM denegado (CHIP no está abierto)</translation>
    </message>
    <message>
        <location filename="../src/diskimages/diskimage.cpp" line="1556"/>
        <source>[%1] Super Archiver Set RAM Buffer</source>
        <translation>[%1] Super Archiver establecido buffer RAM</translation>
    </message>
    <message>
        <location filename="../src/diskimages/diskimage.cpp" line="1567"/>
        <source>[%1] Super Archiver Set RAM Buffer data frame failed.</source>
        <translation>[%1] Error en la trama de datos del buffer RAM del conjunto de Super Archiver.</translation>
    </message>
    <message>
        <location filename="../src/diskimages/diskimage.cpp" line="1576"/>
        <source>[%1] Chip Execute code denied (CHIP is not open)</source>
        <translation>[%1] Código de ejecución de chip denegado (CHIP no está abierto)</translation>
    </message>
    <message>
        <location filename="../src/diskimages/diskimage.cpp" line="1583"/>
        <source>[%1] Chip Execute code (ignored)</source>
        <translation>[%1] Código de ejecución de chip (ignorado)</translation>
    </message>
    <message>
        <location filename="../src/diskimages/diskimage.cpp" line="1598"/>
        <source>[%1] Chip Execute code data frame failed.</source>
        <translation>[%1] Error en la trama de datos del código de ejecución de chip.</translation>
    </message>
    <message>
        <location filename="../src/diskimages/diskimage.cpp" line="1609"/>
        <source>[%1] Get PERCOM block (%2).</source>
        <translation>[%1] Obtener bloque PERCOM (%2).</translation>
    </message>
    <message>
        <location filename="../src/diskimages/diskimage.cpp" line="1625"/>
        <source>[%1] Open CHIP with code aux1 %2, aux2 %2</source>
        <translation>[%1] Abrir CHIP con código aux1 %2, aux2 %2</translation>
    </message>
    <message>
        <location filename="../src/diskimages/diskimage.cpp" line="1627"/>
        <source>[%1] Open CHIP denied on disk %2</source>
        <translation>[%1] CHIP abierto denegado en el disco %2</translation>
    </message>
    <message>
        <location filename="../src/diskimages/diskimage.cpp" line="1632"/>
        <source>[%1] Set PERCOM block (%2).</source>
        <translation>[%1] Establecer bloque PERCOM (%2).</translation>
    </message>
    <message>
        <location filename="../src/diskimages/diskimage.cpp" line="1659"/>
        <source>[%1] BitWriter read memory with AUX=$%2</source>
        <translation>[%1] BitWriter leyó memoria con AUX=$%2</translation>
    </message>
    <message>
        <location filename="../src/diskimages/diskimage.cpp" line="1671"/>
        <source>[%1] Happy Write memory at $%2</source>
        <translation>[%1] Memoria de escritura Happy en $%2</translation>
    </message>
    <message>
        <location filename="../src/diskimages/diskimage.cpp" line="1706"/>
        <source>[%1] Happy %2Write memory at $%3</source>
        <translation>[%1] Happy %2 Escribe memoria a $%3</translation>
    </message>
    <message>
        <location filename="../src/diskimages/diskimage.cpp" line="1706"/>
        <location filename="../src/diskimages/diskimage.cpp" line="1877"/>
        <source>High Speed </source>
        <translation>Alta velocidad</translation>
    </message>
    <message>
        <location filename="../src/diskimages/diskimage.cpp" line="1741"/>
        <source>[%1] Write memory at $%2 NAKed (Address out of range).</source>
        <translation>[%1] Escribir memoria en $%2 NAKed (Dirección fuera de rango).</translation>
    </message>
    <message>
        <location filename="../src/diskimages/diskimage.cpp" line="1751"/>
        <source>[%1] %2Write Sector %3 ($%4) #%5 in track %6 ($%7) with AUX=$%8</source>
        <translation>[%1] %2 Sector de escritura %3 ($%4) #%5 en la pista %6 ($%7) con AUX=$%8</translation>
    </message>
    <message>
        <location filename="../src/diskimages/diskimage.cpp" line="1751"/>
        <location filename="../src/diskimages/diskimage.cpp" line="1753"/>
        <location filename="../src/diskimages/diskimage.cpp" line="1896"/>
        <location filename="../src/diskimages/diskimage.cpp" line="1898"/>
        <source>Happy High Speed </source>
        <translation>Happy alta velocidad</translation>
    </message>
    <message>
        <location filename="../src/diskimages/diskimage.cpp" line="1753"/>
        <source>[%1] %2Write Sector %3 ($%4) #%5 in track %6 ($%7)</source>
        <translation>[%1] %2 Sector de escritura %3 ($%4) #%5 en la pista %6 ($%7)</translation>
    </message>
    <message>
        <location filename="../src/diskimages/diskimage.cpp" line="1765"/>
        <source>[%1] Write sector denied.</source>
        <translation>[%1] Sector de escritura denegado.</translation>
    </message>
    <message>
        <location filename="../src/diskimages/diskimage.cpp" line="1772"/>
        <source>[%1] Write sector failed.</source>
        <translation>[%1] Error en el sector de escritura.</translation>
    </message>
    <message>
        <location filename="../src/diskimages/diskimage.cpp" line="1775"/>
        <source>[%1] Write sector data frame failed.</source>
        <translation>[%1] Error en la trama de datos del sector de escritura.</translation>
    </message>
    <message>
        <location filename="../src/diskimages/diskimage.cpp" line="1780"/>
        <source>[%1] Write sector %2 ($%3) NAKed.</source>
        <translation>[%1] Sector de escritura %2 ($%3) NAKed.</translation>
    </message>
    <message>
        <location filename="../src/diskimages/diskimage.cpp" line="1792"/>
        <source>[%1] Happy Ram check</source>
        <translation>[%1] Happy cheque de Ram</translation>
    </message>
    <message>
        <location filename="../src/diskimages/diskimage.cpp" line="1795"/>
        <source>[%1] Happy Rom 810 test</source>
        <translation>[%1] Prueba Happy Rom 810</translation>
    </message>
    <message>
        <location filename="../src/diskimages/diskimage.cpp" line="1798"/>
        <source>[%1] Happy Extended ram check</source>
        <translation>[%1] Happy verificación de RAM extendida</translation>
    </message>
    <message>
        <location filename="../src/diskimages/diskimage.cpp" line="1801"/>
        <source>[%1] Happy Run diagnostic</source>
        <translation>[%1] Diagnóstico de Happy Run</translation>
    </message>
    <message>
        <location filename="../src/diskimages/diskimage.cpp" line="1804"/>
        <source>[%1] Happy Speed check</source>
        <translation>[%1] Comprobación de velocidad Happy</translation>
    </message>
    <message>
        <location filename="../src/diskimages/diskimage.cpp" line="1808"/>
        <location filename="../src/diskimages/diskimage.cpp" line="2175"/>
        <source>[%1] Happy Step-in / Step-out</source>
        <translation>[%1] Happy paso adentro/afuera</translation>
    </message>
    <message>
        <location filename="../src/diskimages/diskimage.cpp" line="1810"/>
        <source>[%1] Happy Read/Write check</source>
        <translation>[%1] Happy comprobación de lectura/escritura</translation>
    </message>
    <message>
        <location filename="../src/diskimages/diskimage.cpp" line="1813"/>
        <source>[%1] Happy Set unhappy mode</source>
        <translation>[%1] Happy Establecer modo no Happy</translation>
    </message>
    <message>
        <location filename="../src/diskimages/diskimage.cpp" line="1823"/>
        <source>[%1] Happy init backup 1</source>
        <translation>[%1] Happy copia de seguridad de inicio 1</translation>
    </message>
    <message>
        <location filename="../src/diskimages/diskimage.cpp" line="1826"/>
        <source>[%1] Happy init backup 2</source>
        <translation>[%1] Happy copia de seguridad de inicio 2</translation>
    </message>
    <message>
        <location filename="../src/diskimages/diskimage.cpp" line="1832"/>
        <source>[%1] Happy Clear buffer</source>
        <translation>[%1] Happy borrar buffer</translation>
    </message>
    <message>
        <location filename="../src/diskimages/diskimage.cpp" line="1843"/>
        <source>[%1] Happy init backup</source>
        <translation>[%1] Happy copia de seguridad de inicio</translation>
    </message>
    <message>
        <location filename="../src/diskimages/diskimage.cpp" line="1850"/>
        <source>[%1] Command $51 NAKed (HAPPY is not enabled).</source>
        <translation>[%1] Comando $51 NAKed (HAPPY no está habilitado).</translation>
    </message>
    <message>
        <location filename="../src/diskimages/diskimage.cpp" line="1867"/>
        <source>[%1] Happy Read memory at $%2</source>
        <translation>[%1] Memoria de lectura Happy en $%2</translation>
    </message>
    <message>
        <location filename="../src/diskimages/diskimage.cpp" line="1877"/>
        <source>[%1] Happy %2Read memory at $%3</source>
        <translation>[%1] Feliz %2 Read memoria en $%3</translation>
    </message>
    <message>
        <location filename="../src/diskimages/diskimage.cpp" line="1896"/>
        <source>[%1]%2 Read Sector %3 ($%4) #%5 in track %6 ($%7) with AUX=$%8</source>
        <translation>[%1] %2 Sector de lectura %3 ($%4) #%5 en la pista %6 ($%7) con AUX=$%8</translation>
    </message>
    <message>
        <location filename="../src/diskimages/diskimage.cpp" line="1898"/>
        <source>[%1] %2Read Sector %3 ($%4) #%5 in track %6 ($%7)</source>
        <translation>[%1] %2 Read Sector %3 ($%4) #%5 en la pista %6 ($%7)</translation>
    </message>
    <message>
        <location filename="../src/diskimages/diskimage.cpp" line="1928"/>
        <source>[%1] Happy Warp Speed Software V7.1 patched on the fly to be compatible with RespeQt</source>
        <translation>[%1] Happy Warp Speed Software V7.1 parcheado sobre la marcha para que sea compatible con RespeQt</translation>
    </message>
    <message>
        <location filename="../src/diskimages/diskimage.cpp" line="1948"/>
        <source>[%1] Happy Warp Speed Software V5.3 patched on the fly to be compatible with RespeQt</source>
        <translation>[%1] Happy Warp Speed Software V5.3 parcheado sobre la marcha para ser compatible con RespeQt</translation>
    </message>
    <message>
        <location filename="../src/diskimages/diskimage.cpp" line="1964"/>
        <source>[%1] Read sector failed.</source>
        <translation>[%1] Error de lectura del sector.</translation>
    </message>
    <message>
        <location filename="../src/diskimages/diskimage.cpp" line="1972"/>
        <source>[%1] Read sector $%2 NAKed.</source>
        <translation>[%1] Leer sector $%2 NAKed.</translation>
    </message>
    <message>
        <location filename="../src/diskimages/diskimage.cpp" line="1987"/>
        <source>[%1] RespeQt version inquiry: $%2</source>
        <translation>[%1] Consulta sobre la versión de RespeQt: $%2</translation>
    </message>
    <message>
        <location filename="../src/diskimages/diskimage.cpp" line="1989"/>
        <source>[%1] Get status: $%2</source>
        <translation>[%1] Obtener estado: $%2</translation>
    </message>
    <message>
        <location filename="../src/diskimages/diskimage.cpp" line="2006"/>
        <source>[%1] Happy High Speed Write Sector %2 ($%3) #%4 in track %5 ($%6)</source>
        <translation>[%1] Happy sector de escritura de alta velocidad %2 ($%3) #%4 en la pista %5 ($%6)</translation>
    </message>
    <message>
        <location filename="../src/diskimages/diskimage.cpp" line="2017"/>
        <source>[%1] Happy Write sector denied.</source>
        <translation>[%1] Sector de escritura Happy denegado.</translation>
    </message>
    <message>
        <location filename="../src/diskimages/diskimage.cpp" line="2024"/>
        <source>[%1] Happy Write sector failed.</source>
        <translation>[%1] Falló el sector de escritura Happy.</translation>
    </message>
    <message>
        <location filename="../src/diskimages/diskimage.cpp" line="2027"/>
        <source>[%1] Happy Write sector data frame failed.</source>
        <translation>[%1] Error en el marco de datos del sector de escritura Happy.</translation>
    </message>
    <message>
        <location filename="../src/diskimages/diskimage.cpp" line="2060"/>
        <source>[%1] Happy Rom 1050 test</source>
        <translation>[%1] Prueba Happy Rom 1050</translation>
    </message>
    <message>
        <location filename="../src/diskimages/diskimage.cpp" line="2067"/>
        <source>[%1] Get RAM Buffer denied (CHIP is not open)</source>
        <translation>[%1] Obtener buffer de RAM denegado (CHIP no está abierto)</translation>
    </message>
    <message>
        <location filename="../src/diskimages/diskimage.cpp" line="2074"/>
        <source>[%1] Get RAM Buffer</source>
        <translation>[%1] Obtener buffer de RAM</translation>
    </message>
    <message>
        <location filename="../src/diskimages/diskimage.cpp" line="2096"/>
        <source>[%1] Happy High Speed Read Sector %2 ($%3) in track %4 ($%5)</source>
        <translation>[%1] Happy sector de lectura de alta velocidad %2 ($%3) en la pista %4 ($%5)</translation>
    </message>
    <message>
        <location filename="../src/diskimages/diskimage.cpp" line="2110"/>
        <source>[%1] Happy Read Sector failed.</source>
        <translation>[%1] Falló el sector de lectura Happy.</translation>
    </message>
    <message>
        <location filename="../src/diskimages/diskimage.cpp" line="2162"/>
        <source>[%1] Happy Write track %2 ($%3) skew aligned with track %4 ($%5) sector %6 ($%7)</source>
        <translation>[%1] Happy Write track %2 ($%3) sesgo alineado con track %4 ($%5) sector %6 ($%7)</translation>
    </message>
    <message>
        <location filename="../src/diskimages/diskimage.cpp" line="2171"/>
        <source>[%1] Happy init sector copy</source>
        <translation>[%1] Happy copia del sector de inicio</translation>
    </message>
    <message>
        <location filename="../src/diskimages/diskimage.cpp" line="2184"/>
        <source>[%1] Command $%2 NAKed (HAPPY is not enabled).</source>
        <translation>[%1] Commando $%2 NAKed (HAPPY no esta habilitado).</translation>
    </message>
    <message>
        <location filename="../src/diskimages/diskimage.cpp" line="2191"/>
        <source>[%1] Super Archiver Write Fuzzy Sector using Index denied (CHIP is not open)</source>
        <translation>[%1] Super Archiver escribir Fuzzy sector usando índice denegado (CHIP no está abierto)</translation>
    </message>
    <message>
        <location filename="../src/diskimages/diskimage.cpp" line="2198"/>
        <source>[%1] Super Archiver Write Fuzzy Sector using Index with AUX1=$%2 and AUX2=$%3</source>
        <translation>[%1] Super Archiver escribe un sector difuso con índice con AUX1=$%2 y AUX2=$%3</translation>
    </message>
    <message>
        <location filename="../src/diskimages/diskimage.cpp" line="2206"/>
        <source>[%1] Super Archiver Write Fuzzy Sector using Index denied.</source>
        <translation>[%1] Super Archiver escribe Fuzzy Sector usando índice denegado.</translation>
    </message>
    <message>
        <location filename="../src/diskimages/diskimage.cpp" line="2214"/>
        <source>[%1] Super Archiver Write Fuzzy Sector using Index failed.</source>
        <translation>[%1] Error en el sector difuso de escritura del Super Archiver al utilizar el índice.</translation>
    </message>
    <message>
        <location filename="../src/diskimages/diskimage.cpp" line="2217"/>
        <source>[%1] Super Archiver Write Fuzzy Sector using Index data frame failed.</source>
        <translation>[%1] Error en el sector difuso de escritura del Super Archiver utilizando el marco de datos del índice.</translation>
    </message>
    <message>
        <location filename="../src/diskimages/diskimage.cpp" line="2227"/>
        <source>[%1] Format with custom sector skewing.</source>
        <translation>[%1] Formato con sesgo de sector personalizado.</translation>
    </message>
    <message>
        <location filename="../src/diskimages/diskimage.cpp" line="2237"/>
        <source>[%1] Format with custom sector skewing failed.</source>
        <translation>[%1] Error en el formato con sesgo de sector personalizado.</translation>
    </message>
    <message>
        <location filename="../src/diskimages/diskimage.cpp" line="2243"/>
        <source>[%1] Format with custom sector skewing denied.</source>
        <translation>[%1] Formato con sesgo de sector personalizado denegado.</translation>
    </message>
    <message>
        <location filename="../src/diskimages/diskimage.cpp" line="2250"/>
        <source>[%1] Super Archiver Read Track (256 bytes) denied (CHIP is not open)</source>
        <translation>[%1] Super Archiver Read Track (256 bytes) denegado (CHIP no está abierto)</translation>
    </message>
    <message>
        <location filename="../src/diskimages/diskimage.cpp" line="2257"/>
        <source>[%1] Super Archiver Read Track (256 bytes) with AUX1=$%2 and AUX2=$%3</source>
        <translation>[%1] Super Archiver leyendo pista (256 bytes) con AUX1=$%2 y AUX2=$%3</translation>
    </message>
    <message>
        <location filename="../src/diskimages/diskimage.cpp" line="2270"/>
        <source>[%1] Super Archiver Write Fuzzy sector denied (CHIP is not open)</source>
        <translation>[%1] Super Archiver escribiendo Fuzzy sector denegado (CHIP no está abierto)</translation>
    </message>
    <message>
        <location filename="../src/diskimages/diskimage.cpp" line="2281"/>
        <source>[%1] Super Archiver Write Fuzzy Sector %2 ($%3) in track %4 ($%5) with AUX=$%6</source>
        <translation>[%1] Super Archiver escribe sector difuso %2 ($%3) en la pista %4 ($%5) con AUX=$%6</translation>
    </message>
    <message>
        <location filename="../src/diskimages/diskimage.cpp" line="2289"/>
        <source>[%1] Super Archiver Write Fuzzy sector denied.</source>
        <translation>[%1] Se rechazó el sector difuso de escritura del Super Archiver.</translation>
    </message>
    <message>
        <location filename="../src/diskimages/diskimage.cpp" line="2296"/>
        <source>[%1] Super Archiver Write Fuzzy sector failed.</source>
        <translation>[%1] Error en el sector difuso de escritura del Super Archiver.</translation>
    </message>
    <message>
        <location filename="../src/diskimages/diskimage.cpp" line="2299"/>
        <source>[%1] Super Archiver Write Fuzzy sector data frame failed.</source>
        <translation>[%1] Error en la trama de datos del sector difuso de escritura de Super Archiver.</translation>
    </message>
    <message>
        <location filename="../src/diskimages/diskimage.cpp" line="2304"/>
        <source>[%1] Super Archiver Write Fuzzy sector $%2 NAKed.</source>
        <translation>[%1] Super Archiver escribir sector difuso $%2 NAKed.</translation>
    </message>
    <message>
        <location filename="../src/diskimages/diskimage.cpp" line="2311"/>
        <source>[%1] Super Archiver Set Speed denied (CHIP is not open)</source>
        <translation>[%1] Super Archiver Set Speed denegada (CHIP no está abierto)</translation>
    </message>
    <message>
        <location filename="../src/diskimages/diskimage.cpp" line="2319"/>
        <source>[%1] Super Archiver Set Speed %2</source>
        <translation>[%1] Super Archiver Velocidad fijada %2</translation>
    </message>
    <message>
        <location filename="../src/diskimages/diskimage.cpp" line="2329"/>
        <source>[%1] Super Archiver Read Memory denied (CHIP is not open)</source>
        <translation>[%1] Memoria de lectura del Super Archiver denegada (CHIP no está abierto)</translation>
    </message>
    <message>
        <location filename="../src/diskimages/diskimage.cpp" line="2341"/>
        <source>[%1] Super Archiver Read memory (Speed check)</source>
        <translation>[%1] Memoria de lectura de Super Archiver (comprobación de velocidad)</translation>
    </message>
    <message>
        <location filename="../src/diskimages/diskimage.cpp" line="2348"/>
        <source>[%1] Super Archiver Read memory (Diagnostic)</source>
        <translation>[%1] Memoria de lectura del Super Archiver (diagnóstico)</translation>
    </message>
    <message>
        <location filename="../src/diskimages/diskimage.cpp" line="2353"/>
        <source>[%1] Super Archiver Read memory (Address marks)</source>
        <translation>[%1] Memoria de lectura de Super Archiver (marcas de dirección)</translation>
    </message>
    <message>
        <location filename="../src/diskimages/diskimage.cpp" line="2373"/>
        <source>[%1] Super Archiver Read Memory (Skew alignment of $%2)</source>
        <translation>[%1] Memoria de lectura del  Super Archiver (alineación sesgada de $%2)</translation>
    </message>
    <message>
        <location filename="../src/diskimages/diskimage.cpp" line="2375"/>
        <source>[%1] Super Archiver Read Memory (Skew alignment)</source>
        <translation>[%1] Memoria de lectura del Super Archiver (alineación sesgada)</translation>
    </message>
    <message>
        <location filename="../src/diskimages/diskimage.cpp" line="2379"/>
        <source>[%1] Super Archiver Read Memory</source>
        <translation>[%1] Memoria de lectura del Super Archiver</translation>
    </message>
    <message>
        <location filename="../src/diskimages/diskimage.cpp" line="2388"/>
        <source>[%1] Super Archiver Upload and execute code denied (CHIP is not open)</source>
        <translation>[%1] Super Archiver Subir y ejecutar código denegado (CHIP no está abierto)</translation>
    </message>
    <message>
        <location filename="../src/diskimages/diskimage.cpp" line="2396"/>
        <source>[%1] Super Archiver Upload and Execute Code</source>
        <translation>[%1] Código de carga y ejecución de Super Archiver</translation>
    </message>
    <message>
        <location filename="../src/diskimages/diskimage.cpp" line="2416"/>
        <source>[%1] Super Archiver Upload and Execute Code data frame failed.</source>
        <translation>[%1] Error en el marco de datos de carga y ejecución de código de Super Archiver.</translation>
    </message>
    <message>
        <location filename="../src/diskimages/diskimage.cpp" line="2424"/>
        <source>[%1] command: $%2, aux1: $%3, aux2: $%4 NAKed.</source>
        <translation>[%1] commando: $%2, aux1: $%3, aux2: $%4 NAKed.</translation>
    </message>
    <message>
        <location filename="../src/diskimages/diskimage.cpp" line="2517"/>
        <source>[%1] Sending [COMMAND ACK] to Atari</source>
        <translation>[%1] Enviando [COMMAND ACK] al Atari</translation>
    </message>
    <message>
        <location filename="../src/diskimages/diskimage.cpp" line="2524"/>
        <source>[%1] Sending [DATA ACK] to Atari</source>
        <translation>[%1] Enviando [DATA ACK] al Atari</translation>
    </message>
    <message>
        <location filename="../src/diskimages/diskimage.cpp" line="2530"/>
        <source>[%1] Sending [COMMAND NAK] to Atari</source>
        <translation>[%1] Enviando [COMMAND NAK] al Atari</translation>
    </message>
    <message>
        <location filename="../src/diskimages/diskimage.cpp" line="2535"/>
        <source>[%1] Sending [DATA NAK] to Atari</source>
        <translation>[%1] Enviando [DATA NAK] al Atari</translation>
    </message>
    <message>
        <location filename="../src/diskimages/diskimage.cpp" line="2541"/>
        <source>[%1] Sending [COMPLETE] to Atari</source>
        <translation>[%1] Enviando [COMPLETE] al Atari</translation>
    </message>
    <message>
        <location filename="../src/diskimages/diskimage.cpp" line="2547"/>
        <source>[%1] Sending [ERROR] to Atari</source>
        <translation>[%1] Enviando [ERROR] al Atari</translation>
    </message>
    <message>
        <location filename="../src/diskimages/diskimage.cpp" line="2554"/>
        <source>[%1] Receiving %2 bytes from Atari</source>
        <translation>[%1] Recepción %2 bytes de Atari</translation>
    </message>
    <message>
        <location filename="../src/diskimages/diskimage.cpp" line="2562"/>
        <source>[%1] Sending %2 bytes to Atari</source>
        <translation>[%1] Envío %2 bytes de Atari</translation>
    </message>
    <message>
        <location filename="../src/diskimages/diskimage.cpp" line="2653"/>
        <location filename="../src/diskimages/diskimage.cpp" line="2728"/>
        <source>[%1] §%2</source>
        <translation></translation>
    </message>
    <message>
        <location filename="../src/diskimages/diskimage.cpp" line="2692"/>
        <source>[%1] Disassembly of %2 bytes at $%3 with CRC $%4</source>
        <translation>[%1] Desmontaje de %2 bytes en $%3 con CRC $%4</translation>
    </message>
    <message>
        <location filename="../src/diskimages/diskimage.cpp" line="2699"/>
        <source>[%1] §$%2: %3 %4 %5 ; Happy signature</source>
        <translation>[%1] §$%2: %3 %4 %5 ; firma de Happy</translation>
    </message>
    <message>
        <location filename="../src/diskimages/diskimage.cpp" line="2704"/>
        <source>[%1] §$%2: %3 %4 %5 JMP $%6 ; Command $%7</source>
        <translation>[%1] §$%2: %3 %4 %5 JMP $%6 ; Commando $%7</translation>
    </message>
    <message>
        <location filename="../src/diskimages/diskimageatr.cpp" line="48"/>
        <location filename="../src/diskimages/diskimageatx.cpp" line="40"/>
        <location filename="../src/diskimages/diskimagepro.cpp" line="47"/>
        <source>Cannot read the header: %1.</source>
        <translation>No se puede leer el encabezado: %1.</translation>
    </message>
    <message>
        <location filename="../src/diskimages/diskimageatr.cpp" line="57"/>
        <source>Not a valid ATR file.</source>
        <translation>No es un archivo ATR válido.</translation>
    </message>
    <message>
        <location filename="../src/diskimages/diskimageatr.cpp" line="72"/>
        <location filename="../src/diskimages/diskimageatr.cpp" line="220"/>
        <source>Cannot create temporary file &apos;%1&apos;: %2</source>
        <translation>No se puede crear un archivo temporal &apos;%1&apos;: %2</translation>
    </message>
    <message>
        <location filename="../src/diskimages/diskimageatr.cpp" line="83"/>
        <location filename="../src/diskimages/diskimageatr.cpp" line="230"/>
        <location filename="../src/diskimages/diskimagepro.cpp" line="2223"/>
        <location filename="../src/diskimages/diskimagepro.cpp" line="2254"/>
        <source>Cannot read from file: %1.</source>
        <translation>No se puede leer del archivo: %1.</translation>
    </message>
    <message>
        <location filename="../src/diskimages/diskimageatr.cpp" line="90"/>
        <location filename="../src/diskimages/diskimageatr.cpp" line="237"/>
        <source>Cannot write to temporary file &apos;%1&apos;: %2</source>
        <translation>No se puede escribir en el archivo temporal &apos;%1&apos;:%2</translation>
    </message>
    <message>
        <location filename="../src/diskimages/diskimageatr.cpp" line="103"/>
        <source>Image size of &apos;%1&apos; is reported as %2 bytes in the header but it&apos;s actually %3.</source>
        <translation>Tamaño de imagen de &apos;%1&apos; se informa como %2 bytes en el encabezado, pero en realidad es %3.</translation>
    </message>
    <message>
        <location filename="../src/diskimages/diskimageatr.cpp" line="110"/>
        <source>Unknown sector size (%1).</source>
        <translation>Tamaño de sector desconocido (%1).</translation>
    </message>
    <message>
        <location filename="../src/diskimages/diskimageatr.cpp" line="156"/>
        <location filename="../src/diskimages/diskimageatr.cpp" line="248"/>
        <source>Invalid image size (%1).</source>
        <translation>Tamaño de imagen no válido (%1).</translation>
    </message>
    <message>
        <location filename="../src/diskimages/diskimageatr.cpp" line="167"/>
        <source>Too many sectors in the image (%1).</source>
        <translation>Demasiados sectores en la imagen (%1).</translation>
    </message>
    <message>
        <location filename="../src/diskimages/diskimageatr.cpp" line="176"/>
        <source>The file &apos;%1&apos; has some unrecognized fields in its header.</source>
        <translation>El &apos;%1&apos; archivo tiene algunos campos no reconocidos en su encabezado.</translation>
    </message>
    <message>
        <location filename="../src/diskimages/diskimageatr.cpp" line="180"/>
        <source>Cannot resize temporary file &apos;%1&apos;: %2</source>
        <translation>No se puede cambiar el tamaño del archivo temporal &apos;%1&apos;: %2</translation>
    </message>
    <message>
        <location filename="../src/diskimages/diskimageatr.cpp" line="313"/>
        <location filename="../src/diskimages/diskimageatr.cpp" line="385"/>
        <source>Cannot rewind temporary file &apos;%1&apos;: %2</source>
        <translation>No se puede rebobinar el archivo temporal &apos;%1&apos;: %2</translation>
    </message>
    <message>
        <location filename="../src/diskimages/diskimageatr.cpp" line="326"/>
        <location filename="../src/diskimages/diskimageatr.cpp" line="397"/>
        <source>Cannot read from temporary file %1: %2</source>
        <translation>No se puede leer del archivo temporal %1: %2</translation>
    </message>
    <message>
        <location filename="../src/diskimages/diskimageatr.cpp" line="343"/>
        <location filename="../src/diskimages/diskimageatr.cpp" line="491"/>
        <source>Detailed geometry information will be lost when reopening &apos;%1&apos; due to ATR file format limitations.</source>
        <translation>La información detallada de la geometría se perderá al reabrir &apos;%1&apos; debido a las limitaciones del formato de archivo ATR.</translation>
    </message>
    <message>
        <location filename="../src/diskimages/diskimageatr.cpp" line="362"/>
        <source>Detailed disk geometry information will be lost when reopening &apos;%1&apos; due to XFD file format limitations.</source>
        <translation>La información detallada de la geometría del disco se perderá al volver a abrir &apos;%1&apos; debido a las limitaciones del formato de archivo XFD.</translation>
    </message>
    <message>
        <location filename="../src/diskimages/diskimageatr.cpp" line="364"/>
        <source>XFD file format cannot handle this disk geometry. Try saving &apos;%1&apos; as ATR.</source>
        <translation>El formato de archivo XFD no puede manejar esta geometría de disco. Intenta guardar &apos;%1&apos; con ATR.</translation>
    </message>
    <message>
        <location filename="../src/diskimages/diskimageatr.cpp" line="422"/>
        <source>Saving Atr images from the current format is not supported yet.</source>
        <translation>Aún no se admite guardar imágenes Atr del formato actual.</translation>
    </message>
    <message>
        <location filename="../src/diskimages/diskimageatr.cpp" line="476"/>
        <source>Some sector information is lost due to destination file format limitations.</source>
        <translation>Parte de la información del sector se pierde debido a limitaciones de formato de archivo de destino.</translation>
    </message>
    <message>
        <location filename="../src/diskimages/diskimageatr.cpp" line="507"/>
        <source>Cannot create new image: Cannot create temporary file &apos;%2&apos;: %3.</source>
        <translation>No se puede crear una nueva imagen: no se puede crear un archivo temporal &apos;%2&apos;: %3.</translation>
    </message>
    <message>
        <location filename="../src/diskimages/diskimageatr.cpp" line="515"/>
        <location filename="../src/diskimages/diskimageatx.cpp" line="480"/>
        <location filename="../src/diskimages/diskimagepro.cpp" line="452"/>
        <source>Untitled image %1</source>
        <translation>Imagen sin título %1</translation>
    </message>
    <message>
        <location filename="../src/diskimages/diskimageatr.cpp" line="540"/>
        <location filename="../src/diskimages/diskimageatx.cpp" line="544"/>
        <location filename="../src/diskimages/diskimagepro.cpp" line="536"/>
        <source>[%1] Invalid previous track number %2 ($%3) for skew alignment</source>
        <translation>[%1] Número de pista anterior %2 ($%3) no válido para alineación sesgada</translation>
    </message>
    <message>
        <location filename="../src/diskimages/diskimageatr.cpp" line="545"/>
        <location filename="../src/diskimages/diskimageatx.cpp" line="549"/>
        <location filename="../src/diskimages/diskimagepro.cpp" line="541"/>
        <source>[%1] Invalid current track number %2 ($%3) for skew alignment</source>
        <translation>[%1] Número de pista actual no válido %2 ($%3) para alineación sesgada</translation>
    </message>
    <message>
        <location filename="../src/diskimages/diskimageatr.cpp" line="553"/>
        <location filename="../src/diskimages/diskimageatr.cpp" line="562"/>
        <location filename="../src/diskimages/diskimageatx.cpp" line="555"/>
        <location filename="../src/diskimages/diskimageatx.cpp" line="561"/>
        <location filename="../src/diskimages/diskimagepro.cpp" line="576"/>
        <location filename="../src/diskimages/diskimagepro.cpp" line="598"/>
        <source>[%1] Sector %2 ($%3) not found in track %4 ($%5)</source>
        <translation>[%1] El sector %2 ($%3) no se encuentra en la pista %4 ($%5)</translation>
    </message>
    <message>
        <location filename="../src/diskimages/diskimageatr.cpp" line="655"/>
        <location filename="../src/diskimages/diskimageatr.cpp" line="959"/>
        <source>[%1] Write track can not write a non standard track with this kind of image.</source>
        <translation>[%1] Escribir pista no puede escribir una pista no estándar con este tipo de imagen.</translation>
    </message>
    <message>
        <location filename="../src/diskimages/diskimageatr.cpp" line="678"/>
        <source>[%1] command $%2 not supported for sector %3 ($%4) with this kind of image. Ignored.</source>
        <translation>El comando [%1] $%2 no es compatible con el sector %3 ($%4) con este tipo de imagen. Ignorado.</translation>
    </message>
    <message>
        <location filename="../src/diskimages/diskimageatr.cpp" line="681"/>
        <location filename="../src/diskimages/diskimageatx.cpp" line="711"/>
        <location filename="../src/diskimages/diskimagepro.cpp" line="804"/>
        <source>[%1] write unused sector %2 ($%3). Ignored.</source>
        <translation>[%1] escribe el sector %2 no utilizado ($%3). Ignorado.</translation>
    </message>
    <message>
        <location filename="../src/diskimages/diskimageatr.cpp" line="701"/>
        <source>[%1] Cannot format: %2</source>
        <translation>[%1] No se puede formatear :%2</translation>
    </message>
    <message>
        <location filename="../src/diskimages/diskimageatr.cpp" line="713"/>
        <location filename="../src/diskimages/diskimageatr.cpp" line="726"/>
        <source>[%1] Cannot seek to sector %2: %3</source>
        <translation>[%1] No puede buscar el sector %2: %3</translation>
    </message>
    <message>
        <location filename="../src/diskimages/diskimageatr.cpp" line="713"/>
        <source>Sector number is out of bounds.</source>
        <translation>Número sector está fuera del límite.</translation>
    </message>
    <message>
        <location filename="../src/diskimages/diskimageatr.cpp" line="805"/>
        <source>[%1] readSectorStatuses return -1</source>
        <translation>[%1] leer estados del sector devolvió -1</translation>
    </message>
    <message>
        <location filename="../src/diskimages/diskimageatr.cpp" line="844"/>
        <source>[%1] Cannot read from sector %2: %3.</source>
        <translation>[%1] No se puede leer del sector %2: %3.</translation>
    </message>
    <message>
        <location filename="../src/diskimages/diskimageatr.cpp" line="981"/>
        <source>[%1] Fuzzy sectors are not supported with Atr/Xfd file format</source>
        <translation>[%1] Los sectores Fuzzy no son compatibles con el formato de archivo ATR/XFD</translation>
    </message>
    <message>
        <location filename="../src/diskimages/diskimageatr.cpp" line="995"/>
        <source>[%1] Cannot write to sector %2: %3.</source>
        <translation>[%1] No se puede escribir sector %2: %3.</translation>
    </message>
    <message>
        <location filename="../src/diskimages/diskimageatx.cpp" line="49"/>
        <source>Not a valid ATX file.</source>
        <translation>No es un archivo ATX válido.</translation>
    </message>
    <message>
        <location filename="../src/diskimages/diskimageatx.cpp" line="61"/>
        <source>Single</source>
        <translation>Simple</translation>
    </message>
    <message>
        <location filename="../src/diskimages/diskimageatx.cpp" line="64"/>
        <source>Enhanced</source>
        <translation>Mejorada</translation>
    </message>
    <message>
        <location filename="../src/diskimages/diskimageatx.cpp" line="67"/>
        <source>Double</source>
        <translation>Doble</translation>
    </message>
    <message>
        <location filename="../src/diskimages/diskimageatx.cpp" line="70"/>
        <source>Unknown (%1)</source>
        <translation>Desconocido (%1)</translation>
    </message>
    <message>
        <location filename="../src/diskimages/diskimageatx.cpp" line="73"/>
        <source>Track layout for %1. Density is %2</source>
        <translation>Diseño de pista para %1. La densidad es %2</translation>
    </message>
    <message>
        <location filename="../src/diskimages/diskimageatx.cpp" line="82"/>
        <source>[%1] Cannot seek to track header #%2: %3</source>
        <translation>[%1] No se puede buscar para rastrear el encabezado #%2: %3</translation>
    </message>
    <message>
        <location filename="../src/diskimages/diskimageatx.cpp" line="89"/>
        <source>[%1] Track #%2 header could not be read</source>
        <translation>[%1] No se pudo leer el encabezado de la pista #%2</translation>
    </message>
    <message>
        <location filename="../src/diskimages/diskimageatx.cpp" line="95"/>
        <source>[%1] Track header #%2 has an unknown type $%3</source>
        <translation>[%1] El encabezado de la pista #%2 tiene un tipo desconocido $%3</translation>
    </message>
    <message>
        <location filename="../src/diskimages/diskimageatx.cpp" line="110"/>
        <source>[%1] Cannot seek to sector list of track $%2: %3</source>
        <translation>[%1] No se puede buscar la lista de sectores de la pista $%2: %3</translation>
    </message>
    <message>
        <location filename="../src/diskimages/diskimageatx.cpp" line="118"/>
        <source>[%1] Sector List header of track $%2 could not be read</source>
        <translation>[%1] No se pudo leer el encabezado de la lista de sectores de la pista $%2</translation>
    </message>
    <message>
        <location filename="../src/diskimages/diskimageatx.cpp" line="124"/>
        <source>[%1] Sector List header of track $%2 has an unknown type $%3</source>
        <translation>[%1] El encabezado de la lista de sectores de la pista $%2 tiene un tipo desconocido $%3</translation>
    </message>
    <message>
        <location filename="../src/diskimages/diskimageatx.cpp" line="143"/>
        <location filename="../src/diskimages/diskimagepro.cpp" line="145"/>
        <source>%1</source>
        <translation></translation>
    </message>
    <message>
        <location filename="../src/diskimages/diskimageatx.cpp" line="168"/>
        <source>[%1] Cannot seek to sector data of track $%2, sector $%3: %4</source>
        <translation>[%1] No se pueden buscar datos de sector de la pista $%2, sector $%3: %4</translation>
    </message>
    <message>
        <location filename="../src/diskimages/diskimageatx.cpp" line="175"/>
        <source>[%1] Cannot read sector data of track $%2, sector $%3: %4</source>
        <translation>[%1] No se pueden leer los datos del sector de la pista $%2, sector $%3: %4</translation>
    </message>
    <message>
        <location filename="../src/diskimages/diskimageatx.cpp" line="185"/>
        <source>track $%1 (%2): %3</source>
        <translation>pista $%1 (%2): %3</translation>
    </message>
    <message>
        <location filename="../src/diskimages/diskimageatx.cpp" line="192"/>
        <source>[%1] Cannot seek to extended sector data of track $%2: %3</source>
        <translation>[%1] No se pueden buscar datos del sector ampliado de la pista $%2: %3</translation>
    </message>
    <message>
        <location filename="../src/diskimages/diskimageatx.cpp" line="199"/>
        <source>[%1] Extended sector data of track $%2 could not be read</source>
        <translation>[%1] No se pudieron leer los datos del sector extendido de la pista $%2</translation>
    </message>
    <message>
        <location filename="../src/diskimages/diskimageatx.cpp" line="207"/>
        <source>[%1] Extended sector data of track $%2 references an out of bound sector %3</source>
        <translation>[%1] Datos del sector ampliado de la pista $%2 hace referencia a un sector fuera del límite %3</translation>
    </message>
    <message>
        <location filename="../src/diskimages/diskimageatx.cpp" line="415"/>
        <source>Saving Atx images from the current format is not supported yet.</source>
        <translation>Aún no se admite guardar imágenes Atx del formato actual.</translation>
    </message>
    <message>
        <location filename="../src/diskimages/diskimageatx.cpp" line="526"/>
        <source>[%1] Sector %2 ($%3) not found starting at index %4</source>
        <translation>[%1] El sector %2 ($%3) no se encuentra a partir del índice %4</translation>
    </message>
    <message>
        <location filename="../src/diskimages/diskimageatx.cpp" line="649"/>
        <source>[%1] Too many sectors in this track. Ignored.</source>
        <translation>[%1] Demasiados sectores en esta pista. Ignorado.</translation>
    </message>
    <message>
        <location filename="../src/diskimages/diskimageatx.cpp" line="653"/>
        <source>[%1] Special sync header at position %2. Ignored.</source>
        <translation>[%1] Encabezado de sincronización especial en la posición %2. Ignorado.</translation>
    </message>
    <message>
        <location filename="../src/diskimages/diskimageatx.cpp" line="658"/>
        <source>[%1] Header has out of range values: Track=$%2 Sector=$%3. Ignored.</source>
        <translation>[%1] El encabezado tiene valores fuera de rango: Track=$%2 Sector=$%3. Ignorado.</translation>
    </message>
    <message>
        <location filename="../src/diskimages/diskimageatx.cpp" line="708"/>
        <location filename="../src/diskimages/diskimagepro.cpp" line="801"/>
        <source>[%1] sector %2 ($%3) not found. Ignored.</source>
        <translation>[%1] sector %2 ($%3) no encontrado. Ignorado.</translation>
    </message>
    <message>
        <location filename="../src/diskimages/diskimageatx.cpp" line="735"/>
        <location filename="../src/diskimages/diskimageatx.cpp" line="739"/>
        <source>Can not format ATX image: %1</source>
        <translation>No se puede formatear la imagen ATX: %1</translation>
    </message>
    <message>
        <location filename="../src/diskimages/diskimageatx.cpp" line="735"/>
        <location filename="../src/diskimages/diskimagepro.cpp" line="828"/>
        <source>Sector size (%1) not supported (should be 128)</source>
        <translation>Tamaño del sector (%1) no admitido (debería ser 128)</translation>
    </message>
    <message>
        <location filename="../src/diskimages/diskimageatx.cpp" line="739"/>
        <location filename="../src/diskimages/diskimagepro.cpp" line="77"/>
        <location filename="../src/diskimages/diskimagepro.cpp" line="832"/>
        <source>Number of sectors (%1) not supported (max 1040)</source>
        <translation>Número de sectores (%1) no admitidos (máx. 1040)</translation>
    </message>
    <message>
        <location filename="../src/diskimages/diskimageatx.cpp" line="849"/>
        <location filename="../src/diskimages/diskimageatx.cpp" line="876"/>
        <location filename="../src/diskimages/diskimageatx.cpp" line="1513"/>
        <location filename="../src/diskimages/diskimagepro.cpp" line="1133"/>
        <location filename="../src/diskimages/diskimagepro.cpp" line="1160"/>
        <location filename="../src/diskimages/diskimagepro.cpp" line="1847"/>
        <source>[%1] sector layout does not map to track layout</source>
        <translation>[%1] el diseño del sector no se asigna al diseño de la pista</translation>
    </message>
    <message>
        <location filename="../src/diskimages/diskimageatx.cpp" line="885"/>
        <location filename="../src/diskimages/diskimageatx.cpp" line="1521"/>
        <source>[%1] no sector found at index %2 in track %3</source>
        <translation>[%1] no se encontró ningún sector en el índice %2 en la pista %3</translation>
    </message>
    <message>
        <location filename="../src/diskimages/diskimageatx.cpp" line="988"/>
        <location filename="../src/diskimages/diskimagepro.cpp" line="1286"/>
        <source>[%1] Bad sector (status $%2) Grrr Grrr !</source>
        <translation>[%1] Sector defectuoso (estado $%2) ¡Grrr Grrr!</translation>
    </message>
    <message>
        <location filename="../src/diskimages/diskimageatx.cpp" line="994"/>
        <source>[%1] Deleted sector (status $%2)</source>
        <translation>[%1] Sector eliminado (estado $%2)</translation>
    </message>
    <message>
        <location filename="../src/diskimages/diskimageatx.cpp" line="1033"/>
        <location filename="../src/diskimages/diskimagepro.cpp" line="1298"/>
        <source>[%1] Weak sector at offset %2</source>
        <translation>[%1] Sector débil en compensación %2</translation>
    </message>
    <message>
        <location filename="../src/diskimages/diskimageatx.cpp" line="1036"/>
        <location filename="../src/diskimages/diskimagepro.cpp" line="1305"/>
        <source>[%1] CRC error (status $%2) on sector index #%3 among %4 phantom sectors</source>
        <translation>[%1] Error de CRC (estado $%2) en el índice de sector #%3 entre %4 sectores fantasmas</translation>
    </message>
    <message>
        <location filename="../src/diskimages/diskimageatx.cpp" line="1038"/>
        <location filename="../src/diskimages/diskimageatx.cpp" line="1051"/>
        <location filename="../src/diskimages/diskimagepro.cpp" line="1322"/>
        <source>[%1] Read sector index #%2 among %3 phantom sectors</source>
        <translation>[%1] Leer índice de sector #%2 entre %3 sectores fantasmas</translation>
    </message>
    <message>
        <location filename="../src/diskimages/diskimageatx.cpp" line="1041"/>
        <location filename="../src/diskimages/diskimagepro.cpp" line="1312"/>
        <source>[%1] CRC error (status $%2)</source>
        <translation>[%1] Error de CRC (estado $%2)</translation>
    </message>
    <message>
        <location filename="../src/diskimages/diskimageatx.cpp" line="1046"/>
        <location filename="../src/diskimages/diskimagepro.cpp" line="1317"/>
        <source>[%1] Read status $%2</source>
        <translation>[%1] Leer estado $%2</translation>
    </message>
    <message>
        <location filename="../src/diskimages/diskimageatx.cpp" line="1526"/>
        <source>[%1] sector %2 does not match sector number at index %3 in track %4</source>
        <translation>[%1] el sector %2 no coincide con el número de sector en el índice %3 en la pista %4</translation>
    </message>
    <message>
        <location filename="../src/diskimages/diskimageatx.cpp" line="1543"/>
        <source>[%1] Fuzzy sector starting at byte %2</source>
        <translation>[%1] Sector difuso que comienza en el byte %2</translation>
    </message>
    <message>
        <location filename="../src/diskimages/diskimageatx.cpp" line="1546"/>
        <location filename="../src/diskimages/diskimageatx.cpp" line="1619"/>
        <location filename="../src/diskimages/diskimageatx.cpp" line="1692"/>
        <location filename="../src/diskimages/diskimagepro.cpp" line="1877"/>
        <location filename="../src/diskimages/diskimagepro.cpp" line="1965"/>
        <location filename="../src/diskimages/diskimagepro.cpp" line="2079"/>
        <source>[%1] Short sector: %2 bytes</source>
        <translation>[%1] Sector corto: %2 bytes</translation>
    </message>
    <message>
        <location filename="../src/diskimages/diskimageatx.cpp" line="1548"/>
        <location filename="../src/diskimages/diskimageatx.cpp" line="1621"/>
        <location filename="../src/diskimages/diskimageatx.cpp" line="1694"/>
        <location filename="../src/diskimages/diskimagepro.cpp" line="1879"/>
        <location filename="../src/diskimages/diskimagepro.cpp" line="1967"/>
        <location filename="../src/diskimages/diskimagepro.cpp" line="2081"/>
        <source>[%1] CRC error (type $%2)</source>
        <translation>[%1] Error CRC (tipo $%2)</translation>
    </message>
    <message>
        <location filename="../src/diskimages/diskimageatx.cpp" line="1600"/>
        <location filename="../src/diskimages/diskimageatx.cpp" line="1671"/>
        <source>[%1] Sector %2 does not exist in ATX file</source>
        <translation>[%1] El sector %2 no existe en el archivo ATX</translation>
    </message>
    <message>
        <location filename="../src/diskimages/diskimagepro.cpp" line="55"/>
        <source>Not a valid PRO file.</source>
        <translation>No es un archivo válido PRO.</translation>
    </message>
    <message>
        <location filename="../src/diskimages/diskimagepro.cpp" line="61"/>
        <source>Unsupported PRO file version</source>
        <translation>Versión de archivo PRO no compatible</translation>
    </message>
    <message>
        <location filename="../src/diskimages/diskimagepro.cpp" line="70"/>
        <source>Unsupported PRO file size</source>
        <translation>Tamaño de archivo PRO no admitido</translation>
    </message>
    <message>
        <location filename="../src/diskimages/diskimagepro.cpp" line="132"/>
        <source>Track layout for %1</source>
        <translation>Diseño de pista para %1</translation>
    </message>
    <message>
        <location filename="../src/diskimages/diskimagepro.cpp" line="158"/>
        <source>track $%1: %2</source>
        <translation>Pista $%1: %2</translation>
    </message>
    <message>
        <location filename="../src/diskimages/diskimagepro.cpp" line="302"/>
        <source>Saving Pro images from the current format is not supported yet.</source>
        <translation>Aún no se admite guardar imágenes Pro del formato actual.</translation>
    </message>
    <message>
        <location filename="../src/diskimages/diskimagepro.cpp" line="381"/>
        <source>The number of duplicate sectors exceeds the Pro file capacity.</source>
        <translation>El número de sectores duplicados supera la capacidad del archivo Pro.</translation>
    </message>
    <message>
        <location filename="../src/diskimages/diskimagepro.cpp" line="519"/>
        <source>[%1] sector %2 ($%3) not found starting at index %4</source>
        <translation>[%1] sector %2 ($%3) que no se encuentran comenzando en el índice %4</translation>
    </message>
    <message>
        <location filename="../src/diskimages/diskimagepro.cpp" line="699"/>
        <location filename="../src/diskimages/diskimagepro.cpp" line="1635"/>
        <source>[%1] More than 255 phantom sectors (unsupported with PRO format)</source>
        <translation>[%1] Más de 255 sectores fantasmas (no compatibles con el formato PRO)</translation>
    </message>
    <message>
        <location filename="../src/diskimages/diskimagepro.cpp" line="712"/>
        <location filename="../src/diskimages/diskimagepro.cpp" line="1648"/>
        <source>[%1] More than 6 phantom sectors for a given sector number (unsupported with PRO format)</source>
        <translation>[%1] Más de 6 sectores fantasmas para un número de sector determinado (no compatible con el formato PRO)</translation>
    </message>
    <message>
        <location filename="../src/diskimages/diskimagepro.cpp" line="828"/>
        <location filename="../src/diskimages/diskimagepro.cpp" line="832"/>
        <source>Can not format PRO image: %1</source>
        <translation>No se puede formatear la imagen PRO: %1</translation>
    </message>
    <message>
        <location filename="../src/diskimages/diskimagepro.cpp" line="1170"/>
        <location filename="../src/diskimages/diskimagepro.cpp" line="1856"/>
        <source>[%1] sector %2 does not match sector number at index %3</source>
        <translation>[%1] el sector %2 no coincide con el número de sector en el índice %3</translation>
    </message>
    <message>
        <location filename="../src/diskimages/diskimagepro.cpp" line="1252"/>
        <location filename="../src/diskimages/diskimagepro.cpp" line="2047"/>
        <source>[%1] Sector %2 (phantom %3) does not exist in PRO file</source>
        <translation>[%1] El sector %2 (phantom %3) no existe en el archivo PRO</translation>
    </message>
    <message>
        <location filename="../src/diskimages/diskimagepro.cpp" line="1873"/>
        <source>[%1] Fuzzy sector among phantom sectors (unsupported with PRO format)</source>
        <translation>[%1] Sector Fuzzy entre sectores phantom (no compatible con el formato PRO)</translation>
    </message>
    <message>
        <location filename="../src/diskimages/diskimagepro.cpp" line="1917"/>
        <location filename="../src/diskimages/diskimagepro.cpp" line="2036"/>
        <source>[%1] Sector %2 does not exist in PRO file</source>
        <translation>[%1] El sector %2 no existe en el archivo PRO</translation>
    </message>
    <message>
        <location filename="../src/diskimages/diskimagepro.cpp" line="1943"/>
        <source>[%1] Sector can not be fuzzed because all duplicate sector slots are already used</source>
        <translation>[%1] El sector no se puede desviar porque ya se utilizan todos los espacios de sectores duplicados</translation>
    </message>
    <message>
        <location filename="../src/diskimages/diskimagepro.cpp" line="1991"/>
        <location filename="../src/diskimages/diskimagepro.cpp" line="2124"/>
        <location filename="../src/diskimages/diskimagepro.cpp" line="2146"/>
        <source>[%1] Empty duplicate sector slot not found in Pro file</source>
        <translation>[%1] No se encontró el espacio vacío del sector duplicado en el archivo PRO</translation>
    </message>
    <message>
        <location filename="../src/diskimages/diskimagepro.cpp" line="2041"/>
        <source>[%1] Duplicate sector $%2: writing number %3</source>
        <translation>[%1] Sector duplicado $%2: número de escritura %3</translation>
    </message>
    <message>
        <location filename="../src/diskimages/diskimagepro.cpp" line="2242"/>
        <source>[%1] Invalid number of phantom sectors (%2) for sector %3</source>
        <translation>[%1] Número no válido de sectores fantasmas (%2) para el sector %3</translation>
    </message>
    <message>
        <location filename="../src/diskimages/diskimagepro.cpp" line="2289"/>
        <source>[%1] Sector %2 has an invalid phantom index %3.</source>
        <translation>[%1] El sector %2 tiene un índice phantom %3 no válido.</translation>
    </message>
</context>
<context>
    <name>DocDisplayWindow</name>
    <message>
        <location filename="../ui/docdisplaywindow.ui" line="17"/>
        <source>RespeQt User Manual</source>
        <translation>Manual de uso Respeqt</translation>
    </message>
    <message>
        <location filename="../ui/docdisplaywindow.ui" line="35"/>
        <source>qrc:/documentation/RespeQt User Manual-English.html</source>
        <translation>qrc:/documentation/RespeQt User Manual-Spanish.html</translation>
    </message>
    <message>
        <location filename="../ui/docdisplaywindow.ui" line="48"/>
        <source>toolBar</source>
        <translation>Barra de Herramientas</translation>
    </message>
    <message>
        <location filename="../ui/docdisplaywindow.ui" line="76"/>
        <source>Print</source>
        <translation>Imprimir</translation>
    </message>
    <message>
        <location filename="../ui/docdisplaywindow.ui" line="79"/>
        <location filename="../ui/docdisplaywindow.ui" line="82"/>
        <source>Print User Manual</source>
        <translation>Imprimir manual de uso</translation>
    </message>
    <message>
        <location filename="../ui/docdisplaywindow.ui" line="85"/>
        <source>Ctrl+P</source>
        <translation></translation>
    </message>
</context>
<context>
    <name>DriveWidget</name>
    <message>
        <location filename="../ui/drivewidget.ui" line="29"/>
        <source>Widget</source>
        <translation>Panel</translation>
    </message>
    <message>
        <location filename="../ui/drivewidget.ui" line="68"/>
        <source>Frame</source>
        <translation>Cuadro</translation>
    </message>
    <message>
        <location filename="../ui/drivewidget.ui" line="183"/>
        <source>1:</source>
        <translation></translation>
    </message>
    <message>
        <location filename="../ui/drivewidget.ui" line="601"/>
        <source>Mount disk image...</source>
        <translation>Montar imagen de disco...</translation>
    </message>
    <message>
        <location filename="../ui/drivewidget.ui" line="604"/>
        <source>Mount a disk image</source>
        <translation>Montar una imagen de disco</translation>
    </message>
    <message>
        <location filename="../ui/drivewidget.ui" line="607"/>
        <source>Mount a disk image to D%1</source>
        <translation>Montar una imagen de disco en D%1</translation>
    </message>
    <message>
        <location filename="../ui/drivewidget.ui" line="618"/>
        <source>Mount folder image...</source>
        <translation>Montar carpeta de imagen...</translation>
    </message>
    <message>
        <location filename="../ui/drivewidget.ui" line="621"/>
        <source>Mount a folder image</source>
        <translation>Montar una imagen de carpeta</translation>
    </message>
    <message>
        <location filename="../ui/drivewidget.ui" line="624"/>
        <source>Mount a folder image to D%1</source>
        <translation>Montar una imagen de carpeta para D%1</translation>
    </message>
    <message>
        <location filename="../ui/drivewidget.ui" line="638"/>
        <location filename="../ui/drivewidget.ui" line="641"/>
        <source>Unmount</source>
        <translation>Desmontar</translation>
    </message>
    <message>
        <location filename="../ui/drivewidget.ui" line="644"/>
        <source>Unmount D%1</source>
        <translation>Desmontar D%1</translation>
    </message>
    <message>
        <location filename="../ui/drivewidget.ui" line="662"/>
        <source>Write protected</source>
        <translation>Escritura protegida</translation>
    </message>
    <message>
        <location filename="../ui/drivewidget.ui" line="665"/>
        <source>Toggle write protection</source>
        <translation>Alternar protección contra escritura</translation>
    </message>
    <message>
        <location filename="../ui/drivewidget.ui" line="668"/>
        <source>Toggle write protection for D%1</source>
        <translation>Alternar protección de escritura para D%1</translation>
    </message>
    <message>
        <location filename="../ui/drivewidget.ui" line="685"/>
        <source>Explore..</source>
        <translation>Explorar..</translation>
    </message>
    <message>
        <location filename="../ui/drivewidget.ui" line="688"/>
        <source>Show properties</source>
        <translation>Mostrar propiedades</translation>
    </message>
    <message>
        <location filename="../ui/drivewidget.ui" line="691"/>
        <source>Show D%1&apos;s properties</source>
        <translation>Mostrar las propiedades de D%1&apos;s</translation>
    </message>
    <message>
        <location filename="../ui/drivewidget.ui" line="705"/>
        <source>Save</source>
        <translation>Guardar</translation>
    </message>
    <message>
        <location filename="../ui/drivewidget.ui" line="708"/>
        <source>Save image</source>
        <translation>Guardar imagen</translation>
    </message>
    <message>
        <location filename="../ui/drivewidget.ui" line="711"/>
        <source>Save D%1</source>
        <translation>Guardar D%1</translation>
    </message>
    <message>
        <location filename="../ui/drivewidget.ui" line="725"/>
        <source>Revert to original</source>
        <translation>Volver al original</translation>
    </message>
    <message>
        <location filename="../ui/drivewidget.ui" line="728"/>
        <source>Revert image to its last saved state</source>
        <translation>Revertir imagen a su último estado guardado</translation>
    </message>
    <message>
        <location filename="../ui/drivewidget.ui" line="731"/>
        <source>Revert D%1 to its last saved state</source>
        <translation>Revertir D%1 a su último estado guardado</translation>
    </message>
    <message>
        <location filename="../ui/drivewidget.ui" line="745"/>
        <source>Save as...</source>
        <translation>Guardar como...</translation>
    </message>
    <message>
        <location filename="../ui/drivewidget.ui" line="748"/>
        <source>Save to a file</source>
        <translation>Guardar en un archivo</translation>
    </message>
    <message>
        <location filename="../ui/drivewidget.ui" line="751"/>
        <source>Save D%1 to a file</source>
        <translation>Guardar D%1 en un archivo</translation>
    </message>
    <message>
        <location filename="../ui/drivewidget.ui" line="771"/>
        <source>Auto save</source>
        <translation>Guardar automáticamente</translation>
    </message>
    <message>
        <location filename="../ui/drivewidget.ui" line="774"/>
        <source>Commit changes to this disk automatically</source>
        <translation>Confirmar cambios en este disco automáticamente</translation>
    </message>
    <message>
        <location filename="../ui/drivewidget.ui" line="777"/>
        <source>Toggle Auto Commit ON/OFF</source>
        <translation>Activar/Desactivar la activación automática</translation>
    </message>
    <message>
        <location filename="../ui/drivewidget.ui" line="791"/>
        <source>Folder Boot Options</source>
        <translation>Opciones de arranque de carpeta</translation>
    </message>
    <message>
        <location filename="../ui/drivewidget.ui" line="794"/>
        <source>Change Boot Options</source>
        <translation>Cambiar las opciones de arranque</translation>
    </message>
    <message>
        <location filename="../ui/drivewidget.ui" line="812"/>
        <source>Toggle Chip/Super Archiver compatibility</source>
        <translation>Alternar compatibilidad Chip/Super Archiver</translation>
    </message>
    <message>
        <location filename="../ui/drivewidget.ui" line="815"/>
        <source>Enable or disable Chip/Super Archiver compatibility. When enabled, the Chip is
                                open
                            </source>
        <translation>Habilite o deshabilite la compatibilidad con Chip/Super Archiver. Cuando está habilitado, el Chip está
                                abierto</translation>
    </message>
    <message>
        <location filename="../ui/drivewidget.ui" line="835"/>
        <source>Toggle Happy compatibility</source>
        <translation>Alternar compatibilidad Happy</translation>
    </message>
    <message>
        <location filename="../ui/drivewidget.ui" line="838"/>
        <source>Enable or disable Happy compatibility. When enabled, the drive is in Happy mode
                            </source>
        <translation>Habilita o deshabilita la compatibilidad Happy. Cuando está habilitado, la unidad está en modo Happy</translation>
    </message>
    <message>
        <location filename="../ui/drivewidget.ui" line="856"/>
        <location filename="../ui/drivewidget.ui" line="859"/>
        <source>Load next software disk or side</source>
        <translation>Cargue el siguiente disco de software o sid</translation>
    </message>
    <message>
        <location filename="../ui/drivewidget.ui" line="877"/>
        <source>Boot tool disk image</source>
        <translation>Imagen de disco de la herramienta de arranque</translation>
    </message>
    <message>
        <location filename="../ui/drivewidget.ui" line="880"/>
        <source>Boot tool disk image the next time the Atari is powered on</source>
        <translation>Imagen de disco de la herramienta de arranque la próxima vez que se encienda el Atari</translation>
    </message>
    <message>
        <location filename="../ui/drivewidget.ui" line="897"/>
        <source>Boot translator disk image</source>
        <translation>Imagen de disco del traductor de arranque</translation>
    </message>
    <message>
        <location filename="../ui/drivewidget.ui" line="900"/>
        <source>Boot translator disk image the next time the Atari is powered on</source>
        <translation>Arrancar la imagen del disco del traductor la próxima vez que se encienda el Atari</translation>
    </message>
</context>
<context>
    <name>FileModel</name>
    <message>
        <location filename="../src/diskeditdialog.cpp" line="237"/>
        <source>No</source>
        <translation>N°</translation>
    </message>
    <message>
        <location filename="../src/diskeditdialog.cpp" line="240"/>
        <source>Name</source>
        <translation>Nombre</translation>
    </message>
    <message>
        <location filename="../src/diskeditdialog.cpp" line="243"/>
        <source>Extension</source>
        <translation>Extensión</translation>
    </message>
    <message>
        <location filename="../src/diskeditdialog.cpp" line="246"/>
        <source>Size</source>
        <translation>Tamaño</translation>
    </message>
    <message>
        <location filename="../src/diskeditdialog.cpp" line="249"/>
        <source>Time</source>
        <translation>Hora</translation>
    </message>
    <message>
        <location filename="../src/diskeditdialog.cpp" line="252"/>
        <source>Notes</source>
        <translation>Notas</translation>
    </message>
</context>
<context>
    <name>FileTypes</name>
    <message>
        <location filename="../src/miscutils.cpp" line="108"/>
        <source>ATR disk image</source>
        <translation>Imagen de disco ATR</translation>
    </message>
    <message>
        <location filename="../src/miscutils.cpp" line="110"/>
        <source>gzipped ATR disk image</source>
        <translation>imagen de disco ATR comprimida con gzip</translation>
    </message>
    <message>
        <location filename="../src/miscutils.cpp" line="112"/>
        <source>XFD disk image</source>
        <translation>Imagen de disco XFD</translation>
    </message>
    <message>
        <location filename="../src/miscutils.cpp" line="114"/>
        <source>gziped XFD disk image</source>
        <translation>imagen de disco XFD gziped</translation>
    </message>
    <message>
        <location filename="../src/miscutils.cpp" line="116"/>
        <source>DCM disk image</source>
        <translation>Imagen de disco de DCM</translation>
    </message>
    <message>
        <location filename="../src/miscutils.cpp" line="118"/>
        <source>gzipped DCM disk image</source>
        <translation>imagen de disco de DCM comprimida con gzip</translation>
    </message>
    <message>
        <location filename="../src/miscutils.cpp" line="120"/>
        <source>DI disk image</source>
        <translation>Imagen de disco DI</translation>
    </message>
    <message>
        <location filename="../src/miscutils.cpp" line="122"/>
        <source>gzipped DI disk image</source>
        <translation>imagen de disco DI comprimida con gzip</translation>
    </message>
    <message>
        <location filename="../src/miscutils.cpp" line="124"/>
        <source>PRO disk image</source>
        <translation>Imagen de disco PRO</translation>
    </message>
    <message>
        <location filename="../src/miscutils.cpp" line="126"/>
        <source>gzipped PRO disk image</source>
        <translation>Imagen de disco PRO comprimida con gzip</translation>
    </message>
    <message>
        <location filename="../src/miscutils.cpp" line="128"/>
        <source>VAPI (ATX) disk image</source>
        <translation>Imagen de disco VAPI (ATX)</translation>
    </message>
    <message>
        <location filename="../src/miscutils.cpp" line="130"/>
        <source>gzipped VAPI (ATX) disk image</source>
        <translation>Imagen de disco VAPI (ATX) comprimida con gzip</translation>
    </message>
    <message>
        <location filename="../src/miscutils.cpp" line="132"/>
        <source>CAS cassette image</source>
        <translation>Imagen de casete CAS</translation>
    </message>
    <message>
        <location filename="../src/miscutils.cpp" line="134"/>
        <source>gzipped CAS cassette image</source>
        <translation>Imagen de casete CAS comprimida con gzip</translation>
    </message>
    <message>
        <location filename="../src/miscutils.cpp" line="136"/>
        <source>Atari executable</source>
        <translation>Ejecutable de Atari</translation>
    </message>
    <message>
        <location filename="../src/miscutils.cpp" line="138"/>
        <source>gzipped Atari executable</source>
        <translation>ejecutable de Atari comprimido con gzip</translation>
    </message>
    <message>
        <location filename="../src/miscutils.cpp" line="140"/>
        <source>unknown file type</source>
        <translation>Tipo de archivo desconocido</translation>
    </message>
</context>
<context>
    <name>Filesystems::AtariFileSystem</name>
    <message>
        <location filename="../src/filesystems/atarifilesystem.cpp" line="24"/>
        <source>Atari file system error</source>
        <translation>Error del sistema de archivos de Atari</translation>
    </message>
    <message>
        <location filename="../src/filesystems/atarifilesystem.cpp" line="24"/>
        <source>Cannot create directory &apos;%1&apos;.</source>
        <translation>No se puede crear directorio &apos;%1&apos;.</translation>
    </message>
</context>
<context>
    <name>Filesystems::Dos10FileSystem</name>
    <message>
        <location filename="../src/filesystems/dos10filesystem.cpp" line="55"/>
        <location filename="../src/filesystems/dos10filesystem.cpp" line="63"/>
        <location filename="../src/filesystems/dos10filesystem.cpp" line="69"/>
        <location filename="../src/filesystems/dos10filesystem.cpp" line="123"/>
        <location filename="../src/filesystems/dos10filesystem.cpp" line="135"/>
        <location filename="../src/filesystems/dos10filesystem.cpp" line="140"/>
        <location filename="../src/filesystems/dos10filesystem.cpp" line="157"/>
        <location filename="../src/filesystems/dos10filesystem.cpp" line="162"/>
        <location filename="../src/filesystems/dos10filesystem.cpp" line="176"/>
        <location filename="../src/filesystems/dos10filesystem.cpp" line="239"/>
        <location filename="../src/filesystems/dos10filesystem.cpp" line="249"/>
        <location filename="../src/filesystems/dos10filesystem.cpp" line="272"/>
        <location filename="../src/filesystems/dos10filesystem.cpp" line="286"/>
        <location filename="../src/filesystems/dos10filesystem.cpp" line="291"/>
        <location filename="../src/filesystems/dos10filesystem.cpp" line="302"/>
        <location filename="../src/filesystems/dos10filesystem.cpp" line="321"/>
        <location filename="../src/filesystems/dos10filesystem.cpp" line="330"/>
        <location filename="../src/filesystems/dos10filesystem.cpp" line="342"/>
        <location filename="../src/filesystems/dos10filesystem.cpp" line="349"/>
        <location filename="../src/filesystems/dos10filesystem.cpp" line="364"/>
        <location filename="../src/filesystems/dos10filesystem.cpp" line="369"/>
        <location filename="../src/filesystems/dos10filesystem.cpp" line="379"/>
        <location filename="../src/filesystems/dos10filesystem.cpp" line="385"/>
        <location filename="../src/filesystems/dos10filesystem.cpp" line="395"/>
        <location filename="../src/filesystems/dos10filesystem.cpp" line="449"/>
        <location filename="../src/filesystems/dos10filesystem.cpp" line="454"/>
        <location filename="../src/filesystems/dos10filesystem.cpp" line="463"/>
        <source>Atari file system error</source>
        <translation>Error del sistema de archivos de Atari</translation>
    </message>
    <message>
        <location filename="../src/filesystems/dos10filesystem.cpp" line="55"/>
        <source>Cannot create file &apos;%1&apos;.</source>
        <translation>No se puede crear el archivo &apos;%1&apos;.</translation>
    </message>
    <message>
        <location filename="../src/filesystems/dos10filesystem.cpp" line="63"/>
        <location filename="../src/filesystems/dos10filesystem.cpp" line="69"/>
        <source>Cannot read &apos;%1&apos;: %2</source>
        <translation>No puedo leer &apos;%1&apos;: %2</translation>
    </message>
    <message>
        <location filename="../src/filesystems/dos10filesystem.cpp" line="63"/>
        <location filename="../src/filesystems/dos10filesystem.cpp" line="249"/>
        <location filename="../src/filesystems/dos10filesystem.cpp" line="342"/>
        <location filename="../src/filesystems/dos10filesystem.cpp" line="364"/>
        <location filename="../src/filesystems/dos10filesystem.cpp" line="379"/>
        <location filename="../src/filesystems/dos10filesystem.cpp" line="449"/>
        <source>Sector read failed.</source>
        <translation>Sector de lectura ha fallado.</translation>
    </message>
    <message>
        <location filename="../src/filesystems/dos10filesystem.cpp" line="69"/>
        <location filename="../src/filesystems/dos10filesystem.cpp" line="385"/>
        <source>File number mismatch.</source>
        <translation>Número de archivo no coincide.</translation>
    </message>
    <message>
        <location filename="../src/filesystems/dos10filesystem.cpp" line="123"/>
        <source>Cannot write to &apos;%1&apos;: %2</source>
        <translation>No se puede escribir &apos;%1&apos;: %2</translation>
    </message>
    <message>
        <location filename="../src/filesystems/dos10filesystem.cpp" line="135"/>
        <location filename="../src/filesystems/dos10filesystem.cpp" line="140"/>
        <location filename="../src/filesystems/dos10filesystem.cpp" line="162"/>
        <location filename="../src/filesystems/dos10filesystem.cpp" line="176"/>
        <location filename="../src/filesystems/dos10filesystem.cpp" line="239"/>
        <location filename="../src/filesystems/dos10filesystem.cpp" line="249"/>
        <location filename="../src/filesystems/dos10filesystem.cpp" line="272"/>
        <location filename="../src/filesystems/dos10filesystem.cpp" line="286"/>
        <location filename="../src/filesystems/dos10filesystem.cpp" line="291"/>
        <location filename="../src/filesystems/dos10filesystem.cpp" line="302"/>
        <location filename="../src/filesystems/dos10filesystem.cpp" line="321"/>
        <location filename="../src/filesystems/dos10filesystem.cpp" line="330"/>
        <location filename="../src/filesystems/dos10filesystem.cpp" line="342"/>
        <location filename="../src/filesystems/dos10filesystem.cpp" line="349"/>
        <source>Cannot insert &apos;%1&apos;: %2</source>
        <translation>No se puede insertar &apos;%1&apos;: %2</translation>
    </message>
    <message>
        <location filename="../src/filesystems/dos10filesystem.cpp" line="135"/>
        <location filename="../src/filesystems/dos10filesystem.cpp" line="286"/>
        <source>Cannot find a suitable file name.</source>
        <translation>No se puede encontrar un nombre de archivo adecuado.</translation>
    </message>
    <message>
        <location filename="../src/filesystems/dos10filesystem.cpp" line="140"/>
        <location filename="../src/filesystems/dos10filesystem.cpp" line="291"/>
        <source>Directory is full.</source>
        <translation>El disco está lleno.</translation>
    </message>
    <message>
        <location filename="../src/filesystems/dos10filesystem.cpp" line="157"/>
        <source>Cannot open &apos;%1&apos;: %2</source>
        <translation>No se puede abrir &apos;%1&apos;: %2</translation>
    </message>
    <message>
        <location filename="../src/filesystems/dos10filesystem.cpp" line="162"/>
        <location filename="../src/filesystems/dos10filesystem.cpp" line="176"/>
        <location filename="../src/filesystems/dos10filesystem.cpp" line="302"/>
        <location filename="../src/filesystems/dos10filesystem.cpp" line="321"/>
        <source>Disk is full.</source>
        <translation>El disco está lleno.</translation>
    </message>
    <message>
        <location filename="../src/filesystems/dos10filesystem.cpp" line="200"/>
        <source>File system error</source>
        <translation>Error del sistema de archivos</translation>
    </message>
    <message>
        <location filename="../src/filesystems/dos10filesystem.cpp" line="200"/>
        <source>Number of bytes (%1) read from &apos;%2&apos; is not equal to expected data size of (%3)</source>
        <translation>Número de bytes (%1) Leer &apos;%2&apos; No es igual al tamaño de los datos de (%3)</translation>
    </message>
    <message>
        <location filename="../src/filesystems/dos10filesystem.cpp" line="239"/>
        <location filename="../src/filesystems/dos10filesystem.cpp" line="272"/>
        <location filename="../src/filesystems/dos10filesystem.cpp" line="330"/>
        <location filename="../src/filesystems/dos10filesystem.cpp" line="349"/>
        <location filename="../src/filesystems/dos10filesystem.cpp" line="369"/>
        <location filename="../src/filesystems/dos10filesystem.cpp" line="454"/>
        <source>Sector write failed.</source>
        <translation>Sector de escritura ha fallado.</translation>
    </message>
    <message>
        <location filename="../src/filesystems/dos10filesystem.cpp" line="364"/>
        <location filename="../src/filesystems/dos10filesystem.cpp" line="369"/>
        <location filename="../src/filesystems/dos10filesystem.cpp" line="379"/>
        <location filename="../src/filesystems/dos10filesystem.cpp" line="385"/>
        <location filename="../src/filesystems/dos10filesystem.cpp" line="395"/>
        <location filename="../src/filesystems/dos10filesystem.cpp" line="449"/>
        <location filename="../src/filesystems/dos10filesystem.cpp" line="454"/>
        <location filename="../src/filesystems/dos10filesystem.cpp" line="463"/>
        <source>Cannot delete &apos;%1&apos;: %2</source>
        <translation>No se puede eliminar &apos;%1&apos;: %2</translation>
    </message>
    <message>
        <location filename="../src/filesystems/dos10filesystem.cpp" line="395"/>
        <location filename="../src/filesystems/dos10filesystem.cpp" line="463"/>
        <source>Bitmap write failed.</source>
        <translation>No se puede eliminar.</translation>
    </message>
</context>
<context>
    <name>Filesystems::SpartaDosFileSystem</name>
    <message>
        <location filename="../src/filesystems/spartadosfilesystem.cpp" line="123"/>
        <location filename="../src/filesystems/spartadosfilesystem.cpp" line="152"/>
        <source>Atari file system error</source>
        <translation>Error del sistema de archivos de Atari</translation>
    </message>
    <message>
        <location filename="../src/filesystems/spartadosfilesystem.cpp" line="123"/>
        <source>Cannot create file &apos;%1&apos;.</source>
        <translation>No se puede crear el archivo &apos;%1&apos;.</translation>
    </message>
    <message>
        <location filename="../src/filesystems/spartadosfilesystem.cpp" line="152"/>
        <source>Cannot write to &apos;%1&apos;.</source>
        <translation>No se puede escribir &apos;%1&apos;.</translation>
    </message>
</context>
<context>
    <name>GzFile</name>
    <message>
        <location filename="../src/miscutils.cpp" line="162"/>
        <source>gzdopen() failed.</source>
        <translation>gzdopen () falló.</translation>
    </message>
    <message>
        <location filename="../src/miscutils.cpp" line="186"/>
        <source>gzseek() failed.</source>
        <translation>gzseek() falló.</translation>
    </message>
</context>
<context>
    <name>LogDisplayDialog</name>
    <message>
        <location filename="../ui/logdisplaydialog.ui" line="14"/>
        <source>RespeQt Log View</source>
        <translation>RespeQt Registro de Log</translation>
    </message>
    <message>
        <location filename="../ui/logdisplaydialog.ui" line="48"/>
        <source>Filter log by:</source>
        <translation>Filtrar el registro por:</translation>
    </message>
    <message>
        <location filename="../ui/logdisplaydialog.ui" line="64"/>
        <source>ALL</source>
        <translation>Todos</translation>
    </message>
    <message>
        <location filename="../ui/logdisplaydialog.ui" line="69"/>
        <source>Disk 1</source>
        <translation>Disco 1</translation>
    </message>
    <message>
        <location filename="../ui/logdisplaydialog.ui" line="74"/>
        <source>Disk 2</source>
        <translation>Disco 2</translation>
    </message>
    <message>
        <location filename="../ui/logdisplaydialog.ui" line="79"/>
        <source>Disk 3</source>
        <translation>Disco 3</translation>
    </message>
    <message>
        <location filename="../ui/logdisplaydialog.ui" line="84"/>
        <source>Disk 4</source>
        <translation>Disco 4</translation>
    </message>
    <message>
        <location filename="../ui/logdisplaydialog.ui" line="89"/>
        <source>Disk 5</source>
        <translation>Disco 5</translation>
    </message>
    <message>
        <location filename="../ui/logdisplaydialog.ui" line="94"/>
        <source>Disk 6</source>
        <translation>Disco 6</translation>
    </message>
    <message>
        <location filename="../ui/logdisplaydialog.ui" line="99"/>
        <source>Disk 7</source>
        <translation>Disco 7</translation>
    </message>
    <message>
        <location filename="../ui/logdisplaydialog.ui" line="104"/>
        <source>Disk 8</source>
        <translation>Disco 8</translation>
    </message>
    <message>
        <location filename="../ui/logdisplaydialog.ui" line="109"/>
        <source>Disk 9</source>
        <translation>Disco 9</translation>
    </message>
    <message>
        <location filename="../ui/logdisplaydialog.ui" line="114"/>
        <source>Disk 10</source>
        <translation>Disco 10</translation>
    </message>
    <message>
        <location filename="../ui/logdisplaydialog.ui" line="119"/>
        <source>Disk 11</source>
        <translation>Disco 11</translation>
    </message>
    <message>
        <location filename="../ui/logdisplaydialog.ui" line="124"/>
        <source>Disk 12</source>
        <translation>Disco 12</translation>
    </message>
    <message>
        <location filename="../ui/logdisplaydialog.ui" line="129"/>
        <source>Disk 13</source>
        <translation>Disco 13</translation>
    </message>
    <message>
        <location filename="../ui/logdisplaydialog.ui" line="134"/>
        <source>Disk 14</source>
        <translation>Disco 14</translation>
    </message>
    <message>
        <location filename="../ui/logdisplaydialog.ui" line="139"/>
        <source>Disk 15</source>
        <translation>Disco 15</translation>
    </message>
</context>
<context>
    <name>MainWindow</name>
    <message>
        <location filename="../ui/mainwindow.ui" line="190"/>
        <source>&amp;Disk</source>
        <translation>&amp;Disco</translation>
    </message>
    <message>
        <location filename="../ui/mainwindow.ui" line="201"/>
        <source>&amp;File</source>
        <translation>&amp;Archivo</translation>
    </message>
    <message>
        <location filename="../ui/mainwindow.ui" line="216"/>
        <source>&amp;Tools</source>
        <translation>&amp;Herramientas</translation>
    </message>
    <message>
        <location filename="../ui/mainwindow.ui" line="222"/>
        <source>&amp;Help</source>
        <translation>A&amp;yuda</translation>
    </message>
    <message>
        <location filename="../ui/mainwindow.ui" line="232"/>
        <source>&amp;Window</source>
        <translation>&amp;Ventana</translation>
    </message>
    <message>
        <location filename="../ui/mainwindow.ui" line="244"/>
        <source>De&amp;bug</source>
        <translation>&amp;Depurador</translation>
    </message>
    <message>
        <location filename="../ui/mainwindow.ui" line="272"/>
        <source>Unmount &amp;all</source>
        <translation>Desmontar &amp;todos</translation>
    </message>
    <message>
        <location filename="../ui/mainwindow.ui" line="275"/>
        <source>Unmount all</source>
        <translation>Desmontar todos</translation>
    </message>
    <message>
        <location filename="../ui/mainwindow.ui" line="278"/>
        <source>Ctrl+U</source>
        <translation></translation>
    </message>
    <message>
        <location filename="../ui/mainwindow.ui" line="290"/>
        <source>&amp;Options...</source>
        <translation>&amp;Opciones...</translation>
    </message>
    <message>
        <location filename="../ui/mainwindow.ui" line="293"/>
        <location filename="../ui/mainwindow.ui" line="296"/>
        <source>Open options dialog</source>
        <translation>Abrir diálogo de opciones</translation>
    </message>
    <message>
        <location filename="../ui/mainwindow.ui" line="299"/>
        <source>Ctrl+O</source>
        <translation></translation>
    </message>
    <message>
        <location filename="../ui/mainwindow.ui" line="315"/>
        <location filename="../src/mainwindow.cpp" line="931"/>
        <source>&amp;Start emulation</source>
        <translation>&amp;Iniciar emulación</translation>
    </message>
    <message>
        <location filename="../ui/mainwindow.ui" line="318"/>
        <location filename="../ui/mainwindow.ui" line="321"/>
        <location filename="../src/mainwindow.cpp" line="932"/>
        <location filename="../src/mainwindow.cpp" line="933"/>
        <source>Start SIO peripheral emulation</source>
        <translation>Iniciar emulación periférica SIO</translation>
    </message>
    <message>
        <location filename="../ui/mainwindow.ui" line="324"/>
        <source>Alt+E</source>
        <translation></translation>
    </message>
    <message>
        <location filename="../ui/mainwindow.ui" line="335"/>
        <location filename="../ui/mainwindow.ui" line="338"/>
        <source>Mount to the first available slot</source>
        <translation>Monte en la primera ranura disponible</translation>
    </message>
    <message>
        <location filename="../ui/mainwindow.ui" line="347"/>
        <source>Mount &amp;disk image...</source>
        <translation>Montaje &amp;imagen de disco...</translation>
    </message>
    <message>
        <location filename="../ui/mainwindow.ui" line="350"/>
        <location filename="../ui/mainwindow.ui" line="353"/>
        <source>Mount a disk image to the first available slot</source>
        <translation>Monte una imagen de disco en la primera ranura disponible</translation>
    </message>
    <message>
        <location filename="../ui/mainwindow.ui" line="356"/>
        <source>Ctrl+D</source>
        <translation></translation>
    </message>
    <message>
        <location filename="../ui/mainwindow.ui" line="365"/>
        <source>Mount &amp;folder image...</source>
        <translation>Montar &amp;imagen de carpeta...</translation>
    </message>
    <message>
        <location filename="../ui/mainwindow.ui" line="368"/>
        <location filename="../ui/mainwindow.ui" line="371"/>
        <source>Mount a folder image to the first available slot</source>
        <translation>Monte una imagen de carpeta en la primera ranura disponible</translation>
    </message>
    <message>
        <location filename="../ui/mainwindow.ui" line="374"/>
        <source>Ctrl+F</source>
        <translation></translation>
    </message>
    <message>
        <location filename="../ui/mainwindow.ui" line="383"/>
        <source>New disk image...</source>
        <translation>Nueva imagen de disco...</translation>
    </message>
    <message>
        <location filename="../ui/mainwindow.ui" line="386"/>
        <location filename="../ui/mainwindow.ui" line="389"/>
        <source>Create a new disk image file and mount it to the first available slot</source>
        <translation>Cree un nuevo archivo de imagen de disco y móntelo en la primera ranura disponible</translation>
    </message>
    <message>
        <location filename="../ui/mainwindow.ui" line="392"/>
        <source>Ctrl+N</source>
        <translation></translation>
    </message>
    <message>
        <location filename="../ui/mainwindow.ui" line="404"/>
        <source>&amp;Save session</source>
        <translation>&amp;Guardar la sesión</translation>
    </message>
    <message>
        <location filename="../ui/mainwindow.ui" line="407"/>
        <source>Save current session to a file</source>
        <translation>Guardar la sesión actual en un archivo</translation>
    </message>
    <message>
        <location filename="../ui/mainwindow.ui" line="410"/>
        <source>Save current session</source>
        <translation>Guardar sesión actual</translation>
    </message>
    <message>
        <location filename="../ui/mainwindow.ui" line="413"/>
        <source>Alt+S</source>
        <translation></translation>
    </message>
    <message>
        <location filename="../ui/mainwindow.ui" line="422"/>
        <source>&amp;Open session...</source>
        <translation>&amp;Abrir Sesión...</translation>
    </message>
    <message>
        <location filename="../ui/mainwindow.ui" line="425"/>
        <location filename="../ui/mainwindow.ui" line="428"/>
        <source>Open a previously saved session</source>
        <translation>Abre una sesión guardada previamente</translation>
    </message>
    <message>
        <location filename="../ui/mainwindow.ui" line="431"/>
        <source>Alt+O</source>
        <translation></translation>
    </message>
    <message>
        <location filename="../ui/mainwindow.ui" line="440"/>
        <source>&amp;Boot Atari executable...</source>
        <translation>&amp;Inicia ejecutable Atari...</translation>
    </message>
    <message>
        <location filename="../ui/mainwindow.ui" line="443"/>
        <location filename="../ui/mainwindow.ui" line="446"/>
        <source>Boot an Atari executable</source>
        <translation>Inicia un ejecutable Atari</translation>
    </message>
    <message>
        <location filename="../ui/mainwindow.ui" line="449"/>
        <source>Alt+B</source>
        <translation></translation>
    </message>
    <message>
        <location filename="../ui/mainwindow.ui" line="467"/>
        <source>Save</source>
        <translation>Salvar</translation>
    </message>
    <message>
        <location filename="../ui/mainwindow.ui" line="470"/>
        <location filename="../ui/mainwindow.ui" line="473"/>
        <source>Save D1</source>
        <translation>Salvar D1</translation>
    </message>
    <message>
        <location filename="../ui/mainwindow.ui" line="485"/>
        <location filename="../ui/mainwindow.ui" line="488"/>
        <location filename="../ui/mainwindow.ui" line="491"/>
        <source>Show printer text output</source>
        <translation>Mostrar salida de texto de impresora</translation>
    </message>
    <message>
        <location filename="../ui/mainwindow.ui" line="494"/>
        <source>Alt+Shift+T</source>
        <translation></translation>
    </message>
    <message>
        <location filename="../ui/mainwindow.ui" line="503"/>
        <source>Playback cassette image...</source>
        <translation>Reproducción de imagen de cassette...</translation>
    </message>
    <message>
        <location filename="../ui/mainwindow.ui" line="506"/>
        <location filename="../ui/mainwindow.ui" line="509"/>
        <source>Playback a cassette image</source>
        <translation>Reproducción de una imagen de cassette</translation>
    </message>
    <message>
        <location filename="../ui/mainwindow.ui" line="512"/>
        <source>Alt+C</source>
        <translation></translation>
    </message>
    <message>
        <location filename="../ui/mainwindow.ui" line="521"/>
        <source>&amp;Quit</source>
        <translation>&amp;Salir</translation>
    </message>
    <message>
        <location filename="../ui/mainwindow.ui" line="524"/>
        <source>Quit RespeQt</source>
        <translation>Salir de RespeQt</translation>
    </message>
    <message>
        <location filename="../ui/mainwindow.ui" line="527"/>
        <source>Alt+Q</source>
        <translation></translation>
    </message>
    <message>
        <location filename="../ui/mainwindow.ui" line="536"/>
        <source>&amp;About RespeQt</source>
        <translation>&amp;Acerca de RespeQt</translation>
    </message>
    <message>
        <location filename="../ui/mainwindow.ui" line="539"/>
        <source>Ctrl+A</source>
        <translation></translation>
    </message>
    <message>
        <location filename="../ui/mainwindow.ui" line="544"/>
        <source>Help</source>
        <translation>Ayuda</translation>
    </message>
    <message>
        <location filename="../ui/mainwindow.ui" line="552"/>
        <source>Contents</source>
        <translation>Contenido</translation>
    </message>
    <message>
        <location filename="../ui/mainwindow.ui" line="557"/>
        <source>Index</source>
        <translation>Índice</translation>
    </message>
    <message>
        <location filename="../ui/mainwindow.ui" line="569"/>
        <location filename="../ui/mainwindow.ui" line="572"/>
        <location filename="../ui/mainwindow.ui" line="575"/>
        <source>User Manual</source>
        <translation>Manual de usuario</translation>
    </message>
    <message>
        <location filename="../ui/mainwindow.ui" line="578"/>
        <source>Ctrl+Shift+U</source>
        <translation></translation>
    </message>
    <message>
        <location filename="../ui/mainwindow.ui" line="587"/>
        <source>Save mounted image group as default</source>
        <translation>Guardar el grupo de imágenes montadas como predeterminado</translation>
    </message>
    <message>
        <location filename="../ui/mainwindow.ui" line="602"/>
        <location filename="../ui/mainwindow.ui" line="605"/>
        <location filename="../src/mainwindow.cpp" line="894"/>
        <location filename="../src/mainwindow.cpp" line="895"/>
        <source>Stop printer emulation</source>
        <translation>Detener la emulación de la impresora</translation>
    </message>
    <message>
        <location filename="../ui/mainwindow.ui" line="608"/>
        <source>Alt+P</source>
        <translation></translation>
    </message>
    <message>
        <location filename="../ui/mainwindow.ui" line="617"/>
        <location filename="../src/mainwindow.cpp" line="857"/>
        <location filename="../src/mainwindow.cpp" line="858"/>
        <source>Hide drives D9-DO</source>
        <translation>Ocultar unidades de disco D9-DO</translation>
    </message>
    <message>
        <location filename="../ui/mainwindow.ui" line="620"/>
        <source>Alt+H</source>
        <translation></translation>
    </message>
    <message>
        <location filename="../ui/mainwindow.ui" line="629"/>
        <source>Toggle mini mode</source>
        <translation>Modo mini de ventana</translation>
    </message>
    <message>
        <location filename="../ui/mainwindow.ui" line="632"/>
        <source>Ctrl+M</source>
        <translation></translation>
    </message>
    <message>
        <location filename="../ui/mainwindow.ui" line="644"/>
        <source>Toggle shade mode</source>
        <translation>Cambiar modo de sombra</translation>
    </message>
    <message>
        <location filename="../ui/mainwindow.ui" line="647"/>
        <source>Ctrl+S</source>
        <translation></translation>
    </message>
    <message>
        <location filename="../ui/mainwindow.ui" line="656"/>
        <location filename="../ui/mainwindow.ui" line="659"/>
        <location filename="../ui/mainwindow.ui" line="662"/>
        <source>Open log window</source>
        <translation>Abrir ventana de log</translation>
    </message>
    <message>
        <location filename="../ui/mainwindow.ui" line="665"/>
        <source>Ctrl+L</source>
        <translation></translation>
    </message>
    <message>
        <location filename="../ui/mainwindow.ui" line="673"/>
        <source>&amp;Capture snapshot</source>
        <translation>&amp;Capturar instantánea</translation>
    </message>
    <message>
        <location filename="../ui/mainwindow.ui" line="681"/>
        <source>&amp;Replay snapshot</source>
        <translation>&amp;Repetir instantánea</translation>
    </message>
    <message>
        <location filename="../ui/mainwindow.ui" line="686"/>
        <source>Limit file entries</source>
        <translation>Limitar entradas de archivos</translation>
    </message>
    <message>
        <location filename="../ui/mainwindow.ui" line="689"/>
        <source>Limit file entries to 64</source>
        <translation>Limitar las entradas de archivo a 64</translation>
    </message>
    <message>
        <location filename="../ui/mainwindow.ui" line="698"/>
        <source>Open Disk Collection Browser</source>
        <translation>Abrir el explorador de colecciones de discos</translation>
    </message>
    <message>
        <location filename="../ui/mainwindow.ui" line="701"/>
        <location filename="../ui/mainwindow.ui" line="704"/>
        <source>Open/Browse a folder with a collection of disk images</source>
        <translation>Abrir o explorar una carpeta con una colección de imágenes de disco</translation>
    </message>
    <message>
        <location filename="../ui/mainwindow.ui" line="707"/>
        <source>Ctrl+Shift+D</source>
        <translation></translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="166"/>
        <source>RespeQt started @ %1.</source>
        <translation>RespeQt comenzó en %1.</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="238"/>
        <location filename="../src/mainwindow.cpp" line="248"/>
        <source>Session file error</source>
        <translation>Error de archivo de sesión</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="239"/>
        <source>Requested session file not found in the given directory path or the path is incorrect. RespeQt will continue with default session configuration.</source>
        <translation>El archivo de sesión solicitado no se encuentra en la ruta del directorio dado o la ruta es incorrecta. RespeQt continuará con la configuración de sesión predeterminada.</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="249"/>
        <source>Requested session file not found in the application&apos;s current directory path
 (No path was specified). RespeQt will continue with default session configuration.</source>
        <translation>El archivo de sesión solicitado no se encuentra en la ruta del directorio actual de la aplicación
 (No se especificó ninguna ruta). RespeQt continuará con la configuración de sesión predeterminada.</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="260"/>
        <source>RespeQt - Atari Serial Peripheral Emulator for Qt</source>
        <translation>RespeQt - Atari Serial Peripheral Emulator para Qt</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="262"/>
        <location filename="../src/mainwindow.cpp" line="1851"/>
        <source> -- Session: </source>
        <translation> -- Sesión: </translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="274"/>
        <source>19200 bits/sec</source>
        <translation></translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="280"/>
        <source>No TNFS connection</source>
        <translation>Sin conexión TNFS</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="288"/>
        <source>Clear messages</source>
        <translation>Eliminar mensajes</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="295"/>
        <source>Should the file entry limit be 64.</source>
        <translation></translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="372"/>
        <source>RespeQt stopped at %1.</source>
        <translation>RespeQt se detuvo en %1.</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="532"/>
        <source>Swapped disk %1 with disk %2.</source>
        <translation>Disco intercambiado %1 con disco %2.</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="586"/>
        <source>Cannot mount &apos;%1&apos;: No empty disk slots.</source>
        <translation>No puede montar &apos;%1&apos;: No hay ranuras de disco vacías.</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="705"/>
        <source>First run</source>
        <translation>Primer intento</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="706"/>
        <source>You are running RespeQt for the first time.

Do you want to open the options dialog?</source>
        <translation>Está ejecutando RespeQt por primera vez.

ṡQuieres abrir el diálogo de opciones?</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="862"/>
        <location filename="../src/mainwindow.cpp" line="863"/>
        <source>Show drives D9-DO</source>
        <translation>Mostrar unidades D9-DO</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="884"/>
        <source>Printer emulation stopped.</source>
        <translation>La emulación de la impresora se detuvo.</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="888"/>
        <source>Printer emulation started.</source>
        <translation>Emulación de la impresora iniciada.</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="899"/>
        <location filename="../src/mainwindow.cpp" line="900"/>
        <source>Start printer emulation</source>
        <translation>Iniciar la emulación de la impresora</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="919"/>
        <source>&amp;Stop emulation</source>
        <translation>&amp;Detener la emulación</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="920"/>
        <location filename="../src/mainwindow.cpp" line="921"/>
        <source>Stop SIO peripheral emulation</source>
        <translation>Detener la emulación periférica SIO</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="940"/>
        <source>Emulation stopped.</source>
        <translation>La emulación se detuvo.</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="961"/>
        <location filename="../src/mainwindow.cpp" line="981"/>
        <source>Folder image</source>
        <translation>Imagen de carpeta</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="994"/>
        <location filename="../src/mainwindow.cpp" line="1600"/>
        <location filename="../src/mainwindow.cpp" line="1655"/>
        <location filename="../src/mainwindow.cpp" line="1693"/>
        <source>Save failed</source>
        <translation>Error al guardar</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="995"/>
        <location filename="../src/mainwindow.cpp" line="1600"/>
        <location filename="../src/mainwindow.cpp" line="1655"/>
        <location filename="../src/mainwindow.cpp" line="1693"/>
        <source>&apos;%1&apos; cannot be saved, do you want to save the image with another name?</source>
        <translation>&apos;%1&apos; no se puede guardar, ṡdesea guardar la imagen con otro nombre?</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="1248"/>
        <source>Unmounted disk %1</source>
        <translation>Disco %1 desmontado</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="1334"/>
        <source>Open executable</source>
        <translation>Abrir Ejecutable</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="1336"/>
        <source>Atari executables (*.xex *.com *.exe);;All files (*)</source>
        <translation>Atari ejecutable (*.xex *.com *.exe);;All files (*)</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="1424"/>
        <source>Bad cast for PCLINK</source>
        <translation>Cast incorrecto para PCLINK</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="1443"/>
        <location filename="../src/mainwindow.cpp" line="1824"/>
        <source>[%1] Mounted &apos;%2&apos; as &apos;%3&apos;.</source>
        <translation>[%1] Montado &apos;%2&apos; como &apos;%3&apos;.</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="1472"/>
        <source>Open a disk image</source>
        <translation>Abrir una imagen de disco</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="1474"/>
        <location filename="../src/mainwindow.cpp" line="1677"/>
        <source>All Atari disk images (*.atr *.xfd *.atx *.pro);;SIO2PC ATR images (*.atr);;XFormer XFD images (*.xfd);;ATX images (*.atx);;Pro images (*.pro);;All files (*)</source>
        <translation>Todas las imágenes de disco de Atari (*.atr *.xfd *.pro);;Imágenes de SIO2PC ATR (*.atr);;Imágenes de XFormer XFD (*.xfd);;Imágenes PRO (*.pro);;Todos los archivos (*)</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="1498"/>
        <source>Open a folder image</source>
        <translation>Abrir una imagen de carpeta</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="1562"/>
        <source>Image file unsaved</source>
        <translation>Archivo de imagen no guardado</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="1562"/>
        <source>&apos;%1&apos; has unsaved changes, do you want to save it?</source>
        <translation>&apos;%1&apos; tiene cambios no guardados, ṡquieres guardarlo?</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="1644"/>
        <source>[Disk %1] Auto-commit ON.</source>
        <translation>[Disco %1] Activar automáticamente.</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="1646"/>
        <source>[Disk %1] Auto-commit OFF.</source>
        <translation>[Disco %1] Desactivada automáticamente.</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="1675"/>
        <source>Save image as</source>
        <translation>Guardar imagen como</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="1709"/>
        <source>Revert to last saved</source>
        <translation>Volver al último guardado</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="1710"/>
        <source>Do you really want to revert &apos;%1&apos; to its last saved state? You will lose the changes that has been made.</source>
        <translation>¿Realmente desea revertir &apos;%1&apos; a su ultimo estado guardado? Perderás los cambios que se hayan realizado.</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="1830"/>
        <source>Open session</source>
        <translation>Abrir session</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="1832"/>
        <location filename="../src/mainwindow.cpp" line="1868"/>
        <source>RespeQt sessions (*.respeqt);;All files (*)</source>
        <translation>Sesiones de RespeQt (*.respeqt);;Todos los archivos (*)</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="1866"/>
        <source>Save session as</source>
        <translation>Guardar sesión como</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="1904"/>
        <source>Open a cassette image</source>
        <translation>Abrir imagen casete</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="1906"/>
        <source>CAS images (*.cas);;All files (*)</source>
        <translation>Imagenes CAS (*.cas);;All files (*)</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="1994"/>
        <location filename="../src/mainwindow.cpp" line="2014"/>
        <source>Save test Json File</source>
        <translation>Guardar archivo JSON de prueba</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="1994"/>
        <location filename="../src/mainwindow.cpp" line="2014"/>
        <source>Json Files (*.json)</source>
        <translation>Archivos JSON (*.json)</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="2007"/>
        <source>Disconnect serial</source>
        <translation>Desconectar serial</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="2007"/>
        <source>If you proceed, the standard serial port will be disconnected. Would you like to proceed?</source>
        <translation>Si continúa, se desconectará el puerto serial estándar. ¿Desea continuar?</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="2083"/>
        <source>Data Format Conversion Required</source>
        <translation>Se requiere conversión de formato de datos</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="2084"/>
        <source>The Disk Collection Browser Window is currently open.
To convert DCB data format, please close and try again.</source>
        <translation>La ventana del explorador de la colección de discos está abierta actualmente.
Para convertir el formato de datos DCB, ciérrela y vuelva a intentarlo.</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="2097"/>
        <source>No TNFS connections</source>
        <translation>Sin conexiones TNFS</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="2104"/>
        <source>TNFS connected</source>
        <translation>TNFS conectado</translation>
    </message>
</context>
<context>
    <name>OptionsDialog</name>
    <message>
        <location filename="../ui/optionsdialog.ui" line="35"/>
        <source>Options</source>
        <translation>Opciones</translation>
    </message>
    <message>
        <location filename="../ui/optionsdialog.ui" line="76"/>
        <source>1</source>
        <translation></translation>
    </message>
    <message>
        <location filename="../ui/optionsdialog.ui" line="81"/>
        <source>Serial I/O backends</source>
        <translation>Serial E/S backends</translation>
    </message>
    <message>
        <location filename="../ui/optionsdialog.ui" line="88"/>
        <source>Standard serial port</source>
        <translation>Puerto serial estandar</translation>
    </message>
    <message>
        <location filename="../ui/optionsdialog.ui" line="96"/>
        <source>AtariSIO</source>
        <translation></translation>
    </message>
    <message>
        <location filename="../ui/optionsdialog.ui" line="105"/>
        <location filename="../ui/optionsdialog.ui" line="616"/>
        <source>Emulation</source>
        <translation>Emulación</translation>
    </message>
    <message>
        <location filename="../ui/optionsdialog.ui" line="110"/>
        <source>Disk Collection Browser</source>
        <translation>Navegador de colección de discos</translation>
    </message>
    <message>
        <location filename="../ui/optionsdialog.ui" line="115"/>
        <source>Disk Images</source>
        <translation>Imágenes de disco</translation>
    </message>
    <message>
        <location filename="../ui/optionsdialog.ui" line="122"/>
        <source>Image Options</source>
        <translation>Opciones de imagen</translation>
    </message>
    <message>
        <location filename="../ui/optionsdialog.ui" line="127"/>
        <source>OS-B Emulation</source>
        <translation>Emulación OS-B</translation>
    </message>
    <message>
        <location filename="../ui/optionsdialog.ui" line="132"/>
        <source>Icon Visibility</source>
        <translation>Visibilidad de iconos</translation>
    </message>
    <message>
        <location filename="../ui/optionsdialog.ui" line="137"/>
        <source>Favorite Tool Disk</source>
        <translation>Disco de herramienta favorito</translation>
    </message>
    <message>
        <location filename="../ui/optionsdialog.ui" line="143"/>
        <source>User Interface</source>
        <translation>Interfaz de usuario</translation>
    </message>
    <message>
        <location filename="../ui/optionsdialog.ui" line="148"/>
        <source>Printer Emulation</source>
        <translation>Emulación de impresora</translation>
    </message>
    <message>
        <location filename="../ui/optionsdialog.ui" line="155"/>
        <source>Printer Fixed Width Font</source>
        <translation>Fuente de ancho fijo de impresora</translation>
    </message>
    <message>
        <location filename="../ui/optionsdialog.ui" line="160"/>
        <source>Passthrough Settings</source>
        <translation>Configuración de paso a través</translation>
    </message>
    <message>
        <location filename="../ui/optionsdialog.ui" line="165"/>
        <source>Printer Protocol</source>
        <translation>Protocolo de impresora</translation>
    </message>
    <message>
        <location filename="../ui/optionsdialog.ui" line="170"/>
        <source>1020 Emulation Options</source>
        <translation>Opciones de emulación 1020</translation>
    </message>
    <message>
        <location filename="../ui/optionsdialog.ui" line="214"/>
        <source>Standard serial port backend options</source>
        <translation>Opciones de backend de puerto serie estándar</translation>
    </message>
    <message>
        <location filename="../ui/optionsdialog.ui" line="223"/>
        <location filename="../ui/optionsdialog.ui" line="510"/>
        <source>Use this backend</source>
        <translation>Usa este backend</translation>
    </message>
    <message>
        <location filename="../ui/optionsdialog.ui" line="242"/>
        <source>Port name:</source>
        <translation>Nombre del puerto:</translation>
    </message>
    <message>
        <location filename="../ui/optionsdialog.ui" line="258"/>
        <location filename="../ui/optionsdialog.ui" line="543"/>
        <source>Handshake method:</source>
        <translation>Método Handshake:</translation>
    </message>
    <message>
        <location filename="../ui/optionsdialog.ui" line="266"/>
        <location filename="../ui/optionsdialog.ui" line="551"/>
        <source>RI</source>
        <translation></translation>
    </message>
    <message>
        <location filename="../ui/optionsdialog.ui" line="271"/>
        <location filename="../ui/optionsdialog.ui" line="556"/>
        <source>DSR</source>
        <translation></translation>
    </message>
    <message>
        <location filename="../ui/optionsdialog.ui" line="276"/>
        <location filename="../ui/optionsdialog.ui" line="561"/>
        <source>CTS</source>
        <translation></translation>
    </message>
    <message>
        <location filename="../ui/optionsdialog.ui" line="281"/>
        <source>NONE</source>
        <translation>Ninguna</translation>
    </message>
    <message>
        <location filename="../ui/optionsdialog.ui" line="286"/>
        <source>SOFTWARE (SIO2BT)</source>
        <translation>Porgrama (SIO2BT)</translation>
    </message>
    <message>
        <location filename="../ui/optionsdialog.ui" line="294"/>
        <source>Trigger on falling edge</source>
        <translation>Gatillo en el borde de caída</translation>
    </message>
    <message>
        <location filename="../ui/optionsdialog.ui" line="301"/>
        <source>DTR Control Enable</source>
        <translation>Habilitar control DTR</translation>
    </message>
    <message>
        <location filename="../ui/optionsdialog.ui" line="317"/>
        <source>Write delay [ms]:</source>
        <translation>Retraso de escritura [ms]:</translation>
    </message>
    <message>
        <location filename="../ui/optionsdialog.ui" line="331"/>
        <source>0</source>
        <translation></translation>
    </message>
    <message>
        <location filename="../ui/optionsdialog.ui" line="336"/>
        <source>10</source>
        <translation></translation>
    </message>
    <message>
        <location filename="../ui/optionsdialog.ui" line="341"/>
        <source>20</source>
        <translation></translation>
    </message>
    <message>
        <location filename="../ui/optionsdialog.ui" line="346"/>
        <source>30</source>
        <translation></translation>
    </message>
    <message>
        <location filename="../ui/optionsdialog.ui" line="351"/>
        <source>40</source>
        <translation></translation>
    </message>
    <message>
        <location filename="../ui/optionsdialog.ui" line="356"/>
        <source>50</source>
        <translation></translation>
    </message>
    <message>
        <location filename="../ui/optionsdialog.ui" line="361"/>
        <source>60</source>
        <translation></translation>
    </message>
    <message>
        <location filename="../ui/optionsdialog.ui" line="375"/>
        <source>High speed mode baud rate:</source>
        <translation>Velocidad de transmisión en modo de alta velocidad:</translation>
    </message>
    <message>
        <location filename="../ui/optionsdialog.ui" line="386"/>
        <source>19200 (1x)</source>
        <translation></translation>
    </message>
    <message>
        <location filename="../ui/optionsdialog.ui" line="391"/>
        <source>38400 (2x)</source>
        <translation></translation>
    </message>
    <message>
        <location filename="../ui/optionsdialog.ui" line="396"/>
        <source>57600 (3x)</source>
        <translation></translation>
    </message>
    <message>
        <location filename="../ui/optionsdialog.ui" line="404"/>
        <source>Use non-standard speeds</source>
        <translation>Utilice velocidades no estándar</translation>
    </message>
    <message>
        <location filename="../ui/optionsdialog.ui" line="420"/>
        <source>High speed mode POKEY divisor:</source>
        <translation>Modo de alta velocidad divisor POKEY:</translation>
    </message>
    <message>
        <location filename="../ui/optionsdialog.ui" line="440"/>
        <source>&lt;html&gt;&lt;head/&gt;&lt;body&gt;&lt;p&gt;Complete/Error response delay (μs)&lt;/p&gt;&lt;/body&gt;&lt;/html&gt;</source>
        <translation>&lt;html&gt;&lt;head/&gt;&lt;body&gt;&lt;p&gt;Retraso de respuesta completo/error (μs)&lt;/p&gt;&lt;/body&gt;&lt;/html&gt;</translation>
    </message>
    <message>
        <location filename="../ui/optionsdialog.ui" line="447"/>
        <source>μs</source>
        <translation></translation>
    </message>
    <message>
        <location filename="../ui/optionsdialog.ui" line="501"/>
        <source>AtariSIO backend options</source>
        <translation>Opciones de backend AtariSIO</translation>
    </message>
    <message>
        <location filename="../ui/optionsdialog.ui" line="523"/>
        <source>Device name:</source>
        <translation>Nombre del dispositivo:</translation>
    </message>
    <message>
        <location filename="../ui/optionsdialog.ui" line="530"/>
        <source>/dev/atarisio0</source>
        <translation></translation>
    </message>
    <message>
        <location filename="../ui/optionsdialog.ui" line="622"/>
        <source>URL Submit</source>
        <translation>Enviar URL</translation>
    </message>
    <message>
        <location filename="../ui/optionsdialog.ui" line="641"/>
        <source>Try to restart emulation, when SIO connection is lost</source>
        <translation>Intente reiniciar la emulación cuando se pierda la conexión SIO</translation>
    </message>
    <message>
        <location filename="../ui/optionsdialog.ui" line="709"/>
        <source>RCL client local path</source>
        <translation>Ruta local del cliente RCL</translation>
    </message>
    <message>
        <location filename="../ui/optionsdialog.ui" line="716"/>
        <source>PCLINK:</source>
        <translation></translation>
    </message>
    <message>
        <location filename="../ui/optionsdialog.ui" line="729"/>
        <source>Use high speed executable loader</source>
        <translation>Utilice un cargador ejecutable de alta velocidad</translation>
    </message>
    <message>
        <location filename="../ui/optionsdialog.ui" line="739"/>
        <source>CAPITAL letters in file names</source>
        <translation>Letras mayúsculas en nombres de archivos</translation>
    </message>
    <message>
        <location filename="../ui/optionsdialog.ui" line="767"/>
        <source>        (Required for AtariDOS compatibility)</source>
        <translation>        (Requerido para la compatibilidad con AtariDOS)</translation>
    </message>
    <message>
        <location filename="../ui/optionsdialog.ui" line="786"/>
        <source>Filter out underscore character from file names</source>
        <translation>Filtrar el carácter de subrayado de los nombres de archivos</translation>
    </message>
    <message>
        <location filename="../ui/optionsdialog.ui" line="818"/>
        <source>Smart Device:</source>
        <translation>Dispositivo inteligente:</translation>
    </message>
    <message>
        <location filename="../ui/optionsdialog.ui" line="837"/>
        <source>Use custom baud rate for cassette emulation</source>
        <translation>Utilice una velocidad de baudios personalizada para la emulación de casetes</translation>
    </message>
    <message>
        <location filename="../ui/optionsdialog.ui" line="855"/>
        <source>Folder Images:</source>
        <translation>Imágenes de carpeta:</translation>
    </message>
    <message>
        <location filename="../ui/optionsdialog.ui" line="874"/>
        <source>Limit to 64 file entries</source>
        <translation>Límite de 64 entradas de archivo</translation>
    </message>
    <message>
        <location filename="../ui/optionsdialog.ui" line="912"/>
        <source>Disk Collection Browser Options</source>
        <translation>Opciones del navegador de colección de discos</translation>
    </message>
    <message>
        <location filename="../ui/optionsdialog.ui" line="921"/>
        <source>Floppy Labels</source>
        <translation>Etiquetas para disquetes</translation>
    </message>
    <message>
        <location filename="../ui/optionsdialog.ui" line="927"/>
        <source>Index</source>
        <translation>Índice</translation>
    </message>
    <message>
        <location filename="../ui/optionsdialog.ui" line="934"/>
        <source>Title</source>
        <translation>Título</translation>
    </message>
    <message>
        <location filename="../ui/optionsdialog.ui" line="959"/>
        <location filename="../ui/optionsdialog.ui" line="1080"/>
        <source>B</source>
        <translation></translation>
    </message>
    <message>
        <location filename="../ui/optionsdialog.ui" line="1042"/>
        <location filename="../ui/optionsdialog.ui" line="1123"/>
        <source>I</source>
        <translation></translation>
    </message>
    <message>
        <location filename="../ui/optionsdialog.ui" line="1151"/>
        <location filename="../ui/optionsdialog.ui" line="1176"/>
        <source>■</source>
        <translation></translation>
    </message>
    <message>
        <location filename="../ui/optionsdialog.ui" line="1199"/>
        <source>DB Settings Location</source>
        <translation>Ubicación de la configuración de la base de datos</translation>
    </message>
    <message>
        <location filename="../ui/optionsdialog.ui" line="1226"/>
        <source>Store DB settings with App settings</source>
        <translation>Almacenar la configuración de la base de datos con la configuración de la aplicación</translation>
    </message>
    <message>
        <location filename="../ui/optionsdialog.ui" line="1242"/>
        <source>Subdir .respeqt_db in each folder</source>
        <translation>Subdirectorio .respeqt_db en cada carpeta</translation>
    </message>
    <message>
        <location filename="../ui/optionsdialog.ui" line="1249"/>
        <source>Store DB settings in AppData folder:</source>
        <translation>Almacenar la configuración de la base de datos en la carpeta AppData:</translation>
    </message>
    <message>
        <location filename="../ui/optionsdialog.ui" line="1287"/>
        <location filename="../src/optionsdialog.cpp" line="665"/>
        <source>Copy pics to disk folders</source>
        <translation>Copiar imágenes a carpetas del disco</translation>
    </message>
    <message>
        <location filename="../ui/optionsdialog.ui" line="1297"/>
        <source>Scan and Parse filenames for pics/labels</source>
        <translation>Escanear y analizar nombres de archivos para imágenes/etiquetas</translation>
    </message>
    <message>
        <location filename="../ui/optionsdialog.ui" line="1307"/>
        <source>Favor AppData/Json over filename scheme</source>
        <translation>Favorecer AppData/Json sobre el esquema de nombre de archivo</translation>
    </message>
    <message>
        <location filename="../ui/optionsdialog.ui" line="1348"/>
        <source>Image options</source>
        <translation>Opciones de imagen</translation>
    </message>
    <message>
        <location filename="../ui/optionsdialog.ui" line="1354"/>
        <source>Display SIO protocol (ACK, NAK,...)</source>
        <translation>Mostrar protocolo SIO (ACK, NAK,...)</translation>
    </message>
    <message>
        <location filename="../ui/optionsdialog.ui" line="1361"/>
        <location filename="../ui/optionsdialog.ui" line="2991"/>
        <source>Display data frames (spy mode)</source>
        <translation>Mostrar marcos de datos (modo espía)</translation>
    </message>
    <message>
        <location filename="../ui/optionsdialog.ui" line="1368"/>
        <source>Display uploaded code disassembly</source>
        <translation>Mostrar el desmontaje del código cargado</translation>
    </message>
    <message>
        <location filename="../ui/optionsdialog.ui" line="1375"/>
        <source>Display track layout of protected disks</source>
        <translation>Mostrar diseño de pistas de discos protegidos</translation>
    </message>
    <message>
        <location filename="../ui/optionsdialog.ui" line="1395"/>
        <source>Display command name of empty drive slots</source>
        <translation>Mostrar el nombre del comando de las ranuras de unidades vacías</translation>
    </message>
    <message>
        <location filename="../ui/optionsdialog.ui" line="1430"/>
        <source>User inteface</source>
        <translation>Interfaz de usuario</translation>
    </message>
    <message>
        <location filename="../ui/optionsdialog.ui" line="1442"/>
        <source>Use larger font in drive slot descriptions</source>
        <translation>Utilice fuentes más grandes en las descripciones de las ranuras de las unidades</translation>
    </message>
    <message>
        <location filename="../ui/optionsdialog.ui" line="1452"/>
        <source>Save window positions and sizes</source>
        <translation>Guardar posiciones y tamaños de ventanas</translation>
    </message>
    <message>
        <location filename="../ui/optionsdialog.ui" line="1462"/>
        <source>Enable Shade in Mini Mode by default</source>
        <translation>Habilitar sombra en modo mini de forma predeterminada</translation>
    </message>
    <message>
        <location filename="../ui/optionsdialog.ui" line="1475"/>
        <source>Use native menu bar</source>
        <translation>Usar la barra de menú nativa</translation>
    </message>
    <message>
        <location filename="../ui/optionsdialog.ui" line="1485"/>
        <source>Save D9-DO drive visibility status</source>
        <translation>Guardar el estado de visibilidad de la unidad D9-DO</translation>
    </message>
    <message>
        <location filename="../ui/optionsdialog.ui" line="1495"/>
        <source>Show debug menu</source>
        <translation>Mostrar menú de depuración</translation>
    </message>
    <message>
        <location filename="../ui/optionsdialog.ui" line="1531"/>
        <source>Language:</source>
        <translation>Idioma:</translation>
    </message>
    <message>
        <location filename="../ui/optionsdialog.ui" line="1541"/>
        <source>&lt;html&gt;&lt;head/&gt;&lt;body&gt;&lt;p&gt;&lt;span style=&quot; font-weight:600; color:#c50000;&quot;&gt;Restart application to apply this option.&lt;/span&gt;&lt;/p&gt;&lt;/body&gt;&lt;/html&gt;</source>
        <translation>&lt;html&gt;&lt;head/&gt;&lt;body&gt;&lt;p&gt;&lt;span style=&quot; font-weight:600; color:#c50000;&quot;&gt;Reinicie la aplicación para aplicar esta opción.&lt;/span&gt;&lt;/p&gt;&lt;/body&gt;&lt;/html&gt;</translation>
    </message>
    <message>
        <location filename="../ui/optionsdialog.ui" line="1557"/>
        <source>Minimize to system tray</source>
        <translation>Minimizar a la bandeja del sistema</translation>
    </message>
    <message>
        <location filename="../ui/optionsdialog.ui" line="1586"/>
        <source>Printer fixed width font</source>
        <translation>Fuente de ancho fijo de impresora</translation>
    </message>
    <message>
        <location filename="../ui/optionsdialog.ui" line="1606"/>
        <source>Font selection</source>
        <translation>Selección de fuentes</translation>
    </message>
    <message>
        <location filename="../ui/optionsdialog.ui" line="1619"/>
        <source>TextLabel</source>
        <translation></translation>
    </message>
    <message>
        <location filename="../ui/optionsdialog.ui" line="1634"/>
        <source>
                                    Lorem ipsum dolor sit amet, consetetur sadipscing elitr, sed diam nonumy eirmod tempor invidunt ut labore et dolore magna aliquyam erat, sed diam voluptua. At vero eos et accusam et justo duo dolores et ea rebum. Stet clita kasd gubergren, no sea takimata sanctus est Lorem ipsum dolor sit amet. Lorem ipsum dolor sit amet, consetetur sadipscing elitr, sed diam nonumy eirmod tempor invidunt ut labore et dolore magna aliquyam erat, sed diam voluptua. At vero eos et accusam et justo duo dolores et ea rebum. Stet clita kasd gubergren, no sea takimata sanctus est Lorem ipsum dolor sit amet.
                                  </source>
        <translation></translation>
    </message>
    <message>
        <location filename="../ui/optionsdialog.ui" line="1656"/>
        <source>Passthrough settings</source>
        <translation>Configuración de paso a través</translation>
    </message>
    <message>
        <location filename="../ui/optionsdialog.ui" line="1662"/>
        <source>Raw output device</source>
        <translation>Dispositivo de salida sin procesar</translation>
    </message>
    <message>
        <location filename="../ui/optionsdialog.ui" line="1716"/>
        <source>810 firmware path</source>
        <translation>810 ruta de firmware</translation>
    </message>
    <message>
        <location filename="../ui/optionsdialog.ui" line="1722"/>
        <source>Atari 810 Happy firmware path:</source>
        <translation>Ruta del firmware Atari 810 Happy:</translation>
    </message>
    <message>
        <location filename="../ui/optionsdialog.ui" line="1743"/>
        <source>Atari 810 Chip firmware path:</source>
        <translation>Ruta del firmware del chip Atari 810:</translation>
    </message>
    <message>
        <location filename="../ui/optionsdialog.ui" line="1770"/>
        <source>Atari 810 firmware path:</source>
        <translation>Ruta del firmware del Atari 810:</translation>
    </message>
    <message>
        <location filename="../ui/optionsdialog.ui" line="1880"/>
        <source>1050 firmware path</source>
        <translation>1050 ruta de firmware</translation>
    </message>
    <message>
        <location filename="../ui/optionsdialog.ui" line="1886"/>
        <source>Atari 1050 Turbo firmware path:</source>
        <translation>Ruta del firmware de Atari 1050 Turbo:</translation>
    </message>
    <message>
        <location filename="../ui/optionsdialog.ui" line="1893"/>
        <source>Atari 1050 Duplicator firmware path:</source>
        <translation>Ruta del firmware de la duplicadora Atari 1050:</translation>
    </message>
    <message>
        <location filename="../ui/optionsdialog.ui" line="1937"/>
        <source>Atari 1050 firmware path:</source>
        <translation>Ruta del firmware de Atari 1050:</translation>
    </message>
    <message>
        <location filename="../ui/optionsdialog.ui" line="1951"/>
        <source>Atari 1050 Archiver firmware path:</source>
        <translation>Ruta del firmware del archivador Atari 1050:</translation>
    </message>
    <message>
        <location filename="../ui/optionsdialog.ui" line="1965"/>
        <source>Atari 1050 Happy firmware path:</source>
        <translation>Ruta del firmware Atari 1050 Happy:</translation>
    </message>
    <message>
        <location filename="../ui/optionsdialog.ui" line="1972"/>
        <source>Atari 1050 Speedy firmware path:</source>
        <translation>Ruta del firmware Atari 1050 Speedy:</translation>
    </message>
    <message>
        <location filename="../ui/optionsdialog.ui" line="2149"/>
        <source>Firmware emulation</source>
        <translation>Emulación de firmware</translation>
    </message>
    <message>
        <location filename="../ui/optionsdialog.ui" line="2155"/>
        <source>Modifications in this panel are applied when a</source>
        <translation>Las modificaciones en este panel se aplican cuando un</translation>
    </message>
    <message>
        <location filename="../ui/optionsdialog.ui" line="2162"/>
        <source>disk image is loaded in one of the first 4 drives</source>
        <translation>La imagen del disco se carga en una de las primeras 4 unidades</translation>
    </message>
    <message>
        <location filename="../ui/optionsdialog.ui" line="2170"/>
        <location filename="../ui/optionsdialog.ui" line="2258"/>
        <location filename="../ui/optionsdialog.ui" line="2312"/>
        <location filename="../ui/optionsdialog.ui" line="2366"/>
        <source>SIO command emulation (default)</source>
        <translation>Emulación de comando SIO (predeterminada)</translation>
    </message>
    <message>
        <location filename="../ui/optionsdialog.ui" line="2175"/>
        <location filename="../ui/optionsdialog.ui" line="2263"/>
        <location filename="../ui/optionsdialog.ui" line="2317"/>
        <location filename="../ui/optionsdialog.ui" line="2371"/>
        <source>Atari 810 firmware</source>
        <translation>Firmware del Atari 810</translation>
    </message>
    <message>
        <location filename="../ui/optionsdialog.ui" line="2180"/>
        <location filename="../ui/optionsdialog.ui" line="2268"/>
        <location filename="../ui/optionsdialog.ui" line="2322"/>
        <location filename="../ui/optionsdialog.ui" line="2376"/>
        <source>Atari 810 with the Chip firmware</source>
        <translation>Atari 810 con el firmware de chip</translation>
    </message>
    <message>
        <location filename="../ui/optionsdialog.ui" line="2185"/>
        <location filename="../ui/optionsdialog.ui" line="2273"/>
        <location filename="../ui/optionsdialog.ui" line="2327"/>
        <location filename="../ui/optionsdialog.ui" line="2381"/>
        <source>Atari 810 with Happy firmware</source>
        <translation>Atari 810 con firmware Happy</translation>
    </message>
    <message>
        <location filename="../ui/optionsdialog.ui" line="2190"/>
        <location filename="../ui/optionsdialog.ui" line="2278"/>
        <location filename="../ui/optionsdialog.ui" line="2332"/>
        <location filename="../ui/optionsdialog.ui" line="2386"/>
        <source>Atari 1050 firmware</source>
        <translation>Firmware de Atari 1050</translation>
    </message>
    <message>
        <location filename="../ui/optionsdialog.ui" line="2195"/>
        <location filename="../ui/optionsdialog.ui" line="2283"/>
        <location filename="../ui/optionsdialog.ui" line="2337"/>
        <location filename="../ui/optionsdialog.ui" line="2391"/>
        <source>Atari 1050 with the Archiver firmware</source>
        <translation>Atari 1050 con el firmware Archiver</translation>
    </message>
    <message>
        <location filename="../ui/optionsdialog.ui" line="2200"/>
        <location filename="../ui/optionsdialog.ui" line="2288"/>
        <location filename="../ui/optionsdialog.ui" line="2342"/>
        <location filename="../ui/optionsdialog.ui" line="2396"/>
        <source>Atari 1050 with Happy firmware</source>
        <translation>Atari 1050 con firmware Happy</translation>
    </message>
    <message>
        <location filename="../ui/optionsdialog.ui" line="2205"/>
        <location filename="../ui/optionsdialog.ui" line="2293"/>
        <location filename="../ui/optionsdialog.ui" line="2347"/>
        <location filename="../ui/optionsdialog.ui" line="2401"/>
        <source>Atari 1050 with Speedy firmware</source>
        <translation>Atari 1050 con firmware Speedy</translation>
    </message>
    <message>
        <location filename="../ui/optionsdialog.ui" line="2210"/>
        <location filename="../ui/optionsdialog.ui" line="2298"/>
        <location filename="../ui/optionsdialog.ui" line="2352"/>
        <location filename="../ui/optionsdialog.ui" line="2406"/>
        <source>Atari 1050 with Turbo firmware</source>
        <translation>Atari 1050 con firmware Turbo</translation>
    </message>
    <message>
        <location filename="../ui/optionsdialog.ui" line="2215"/>
        <location filename="../ui/optionsdialog.ui" line="2303"/>
        <location filename="../ui/optionsdialog.ui" line="2357"/>
        <location filename="../ui/optionsdialog.ui" line="2411"/>
        <source>Atari 1050 with Duplicator firmware</source>
        <translation>Atari 1050 con firmware duplicador</translation>
    </message>
    <message>
        <location filename="../ui/optionsdialog.ui" line="2223"/>
        <source>Power on D3 with disk inserted</source>
        <translation>Encienda D3 con el disco insertado</translation>
    </message>
    <message>
        <location filename="../ui/optionsdialog.ui" line="2230"/>
        <source>Power on D2 with disk inserted</source>
        <translation>Encienda D2 con el disco insertado</translation>
    </message>
    <message>
        <location filename="../ui/optionsdialog.ui" line="2237"/>
        <source>Power on D4 with disk inserted</source>
        <translation>Encienda D4 con el disco insertado</translation>
    </message>
    <message>
        <location filename="../ui/optionsdialog.ui" line="2250"/>
        <source>D2:</source>
        <translation></translation>
    </message>
    <message>
        <location filename="../ui/optionsdialog.ui" line="2425"/>
        <source>D3:</source>
        <translation></translation>
    </message>
    <message>
        <location filename="../ui/optionsdialog.ui" line="2451"/>
        <source>D1:</source>
        <translation></translation>
    </message>
    <message>
        <location filename="../ui/optionsdialog.ui" line="2464"/>
        <source>D4</source>
        <translation></translation>
    </message>
    <message>
        <location filename="../ui/optionsdialog.ui" line="2471"/>
        <source>Power on D1 with disk inserted</source>
        <translation>Encienda D1 con el disco insertado</translation>
    </message>
    <message>
        <location filename="../ui/optionsdialog.ui" line="2522"/>
        <source>Trace options</source>
        <translation>Opciones de seguimiento</translation>
    </message>
    <message>
        <location filename="../ui/optionsdialog.ui" line="2528"/>
        <source>Display ID addess marks</source>
        <translation>Marcas de dirección de ID de pantalla</translation>
    </message>
    <message>
        <location filename="../ui/optionsdialog.ui" line="2548"/>
        <source>Trace CPU execution in this file:</source>
        <translation>Rastree la ejecución de la CPU en este archivo:</translation>
    </message>
    <message>
        <location filename="../ui/optionsdialog.ui" line="2555"/>
        <source>Display drive head position</source>
        <translation>Mostrar la posición del cabezal de accionamiento</translation>
    </message>
    <message>
        <location filename="../ui/optionsdialog.ui" line="2569"/>
        <source>Display index pulse</source>
        <translation>Pulso de índice de visualización</translation>
    </message>
    <message>
        <location filename="../ui/optionsdialog.ui" line="2576"/>
        <source>Display track information</source>
        <translation>Mostrar información de la pista</translation>
    </message>
    <message>
        <location filename="../ui/optionsdialog.ui" line="2583"/>
        <source>Display FDC commands</source>
        <translation>Mostrar comandos FDC</translation>
    </message>
    <message>
        <location filename="../ui/optionsdialog.ui" line="2590"/>
        <source>Display motor On/Off</source>
        <translation>Motor de visualización encendido/apagado</translation>
    </message>
    <message>
        <location filename="../ui/optionsdialog.ui" line="2625"/>
        <source>OS-B emulation</source>
        <translation>Emulación OS-B</translation>
    </message>
    <message>
        <location filename="../ui/optionsdialog.ui" line="2631"/>
        <source>With this option active, RespeQt will detect [OS-B] in a disk image filename and will automatically boot a translator disk before booting the chosen disk image.</source>
        <translation>Con esta opción activa, RespeQt detectará [OS-B] en el nombre de un archivo de imagen de disco e iniciará automáticamente un disco traductor antes de iniciar la imagen de disco elegida.</translation>
    </message>
    <message>
        <location filename="../ui/optionsdialog.ui" line="2641"/>
        <source>Translator disk to use with [OS-B] disk images</source>
        <translation>Disco traductor para usar con imágenes de disco [OS-B]</translation>
    </message>
    <message>
        <location filename="../ui/optionsdialog.ui" line="2651"/>
        <source>NOTE: This option applies only to slot D1:</source>
        <translation>NOTA: Esta opción se aplica solo a la ranura D1:</translation>
    </message>
    <message>
        <location filename="../ui/optionsdialog.ui" line="2690"/>
        <source>Detect [OS-B] and boot Translator</source>
        <translation>Detectar [OS-B] y arrancar Translator</translation>
    </message>
    <message>
        <location filename="../ui/optionsdialog.ui" line="2740"/>
        <source>Icon visibility in drive slots</source>
        <translation>Visibilidad de iconos en las ranuras de disco</translation>
    </message>
    <message>
        <location filename="../ui/optionsdialog.ui" line="2746"/>
        <source>Hide Next image icon</source>
        <translation>Ocultar icono de imagen siguiente</translation>
    </message>
    <message>
        <location filename="../ui/optionsdialog.ui" line="2753"/>
        <source>Hide OS-B mode icon</source>
        <translation>Ocultar el icono del modo OS-B</translation>
    </message>
    <message>
        <location filename="../ui/optionsdialog.ui" line="2760"/>
        <source>Hide Chip mode icon</source>
        <translation>Ocultar icono del modo Chip</translation>
    </message>
    <message>
        <location filename="../ui/optionsdialog.ui" line="2767"/>
        <source>Hide Happy mode icon</source>
        <translation>Ocultar icono del modo Happy</translation>
    </message>
    <message>
        <location filename="../ui/optionsdialog.ui" line="2774"/>
        <source>Hide Tool disk icon</source>
        <translation>Ocultar icono de disco de herramientas</translation>
    </message>
    <message>
        <location filename="../ui/optionsdialog.ui" line="2810"/>
        <source>These features can still be toggled with contextual menu.</source>
        <translation>Estas funciones aún se pueden alternar con el menú contextual.</translation>
    </message>
    <message>
        <location filename="../ui/optionsdialog.ui" line="2857"/>
        <source>Favorite tool disk</source>
        <translation>Disco de herramientas favorito</translation>
    </message>
    <message>
        <location filename="../ui/optionsdialog.ui" line="2872"/>
        <source>The favorite tool disk is a disk image to boot before the selected disk image in slot D1:. For example, you can have your favorite copier always ready with this option.</source>
        <translation>El disco de herramientas favorito es una imagen de disco para arrancar antes de la imagen de disco seleccionada en la ranura D1:. Por ejemplo, podrás tener tu fotocopiadora favorita siempre lista con esta opción.</translation>
    </message>
    <message>
        <location filename="../ui/optionsdialog.ui" line="2888"/>
        <source>Tool disk image to boot when the favorite icon is active</source>
        <translation>Imagen de disco de herramientas para arrancar cuando el icono de favorito está activo</translation>
    </message>
    <message>
        <location filename="../ui/optionsdialog.ui" line="2932"/>
        <source>Activate Chip mode when booting this disk</source>
        <translation>Active el modo Chip al iniciar este disco</translation>
    </message>
    <message>
        <location filename="../ui/optionsdialog.ui" line="2939"/>
        <source>Activate Happy mode when booting this disk</source>
        <translation>Active el modo Happy al iniciar este disco</translation>
    </message>
    <message>
        <location filename="../ui/optionsdialog.ui" line="2972"/>
        <source>Printer protocol</source>
        <translation>Protocolo de impresora</translation>
    </message>
    <message>
        <location filename="../ui/optionsdialog.ui" line="3011"/>
        <source>1020 Options</source>
        <translation>1020 Opciones</translation>
    </message>
    <message>
        <location filename="../ui/optionsdialog.ui" line="3017"/>
        <source>Clear Graphics when a STATUS command is received</source>
        <translation>Gráficos claros cuando se recibe un comando STATUS</translation>
    </message>
    <message>
        <location filename="../ui/optionsdialog.ui" line="3037"/>
        <source>Display the Graphics instructions in the log window</source>
        <translation>Mostrar las instrucciones gráficas en la ventana de registro</translation>
    </message>
    <message>
        <location filename="../ui/optionsdialog.ui" line="3055"/>
        <source>Save/Commit or Cancel/Ignore changes made to the settings</source>
        <translation>Guardar/Confirmar o Cancelar/Ignorar los cambios realizados en la configuración</translation>
    </message>
    <message>
        <location filename="../src/optionsdialog.cpp" line="68"/>
        <source>Custom</source>
        <translation>Personalizado</translation>
    </message>
    <message>
        <location filename="../src/optionsdialog.cpp" line="194"/>
        <source>Automatic</source>
        <translation>Atomatico</translation>
    </message>
    <message>
        <location filename="../src/optionsdialog.cpp" line="197"/>
        <location filename="../src/optionsdialog.cpp" line="206"/>
        <source>English</source>
        <translation>Español</translation>
    </message>
    <message>
        <location filename="../src/optionsdialog.cpp" line="565"/>
        <source>Select Atari 810 firmware</source>
        <translation>Seleccione el firmware Atari 810</translation>
    </message>
    <message>
        <location filename="../src/optionsdialog.cpp" line="565"/>
        <location filename="../src/optionsdialog.cpp" line="569"/>
        <location filename="../src/optionsdialog.cpp" line="573"/>
        <location filename="../src/optionsdialog.cpp" line="577"/>
        <location filename="../src/optionsdialog.cpp" line="581"/>
        <location filename="../src/optionsdialog.cpp" line="585"/>
        <location filename="../src/optionsdialog.cpp" line="589"/>
        <location filename="../src/optionsdialog.cpp" line="593"/>
        <location filename="../src/optionsdialog.cpp" line="597"/>
        <source>Atari drive firmware (*.rom);;All files (*)</source>
        <translation>Firmware de la unidad Atari (*.rom);;Todos los archivos (*)</translation>
    </message>
    <message>
        <location filename="../src/optionsdialog.cpp" line="569"/>
        <source>Select Atari 810 Chip firmware</source>
        <translation>Seleccione el firmware del Chip Atari 810</translation>
    </message>
    <message>
        <location filename="../src/optionsdialog.cpp" line="573"/>
        <source>Select Atari 810 Happy firmware</source>
        <translation>Seleccione el firmware Atari 810 Happy</translation>
    </message>
    <message>
        <location filename="../src/optionsdialog.cpp" line="577"/>
        <source>Select Atari 1050 firmware</source>
        <translation>Seleccione el firmware Atari 1050</translation>
    </message>
    <message>
        <location filename="../src/optionsdialog.cpp" line="581"/>
        <source>Select Atari 1050 Archiver firmware</source>
        <translation>Seleccione el firmware del Atari 1050 Archiver</translation>
    </message>
    <message>
        <location filename="../src/optionsdialog.cpp" line="585"/>
        <source>Select Atari 1050 Happy firmware</source>
        <translation>Seleccione el firmware Atari 1050 Happy</translation>
    </message>
    <message>
        <location filename="../src/optionsdialog.cpp" line="589"/>
        <source>Select Atari 1050 Speedy firmware</source>
        <translation>Seleccione el firmware Atari 1050 Speedy</translation>
    </message>
    <message>
        <location filename="../src/optionsdialog.cpp" line="593"/>
        <source>Select Atari 1050 Turbo firmware</source>
        <translation>Seleccione el firmware Atari 1050 Turbo</translation>
    </message>
    <message>
        <location filename="../src/optionsdialog.cpp" line="597"/>
        <source>Select Atari 1050 Duplicator firmware</source>
        <translation>Seleccione el firmware de la duplicadora Atari 1050</translation>
    </message>
    <message>
        <location filename="../src/optionsdialog.cpp" line="602"/>
        <source>Select translator disk image</source>
        <translation>Seleccione la imagen del disco del traductor</translation>
    </message>
    <message>
        <location filename="../src/optionsdialog.cpp" line="602"/>
        <location filename="../src/optionsdialog.cpp" line="606"/>
        <source>Atari disk image (*.atr);;All files (*)</source>
        <translation>Imagen de disco Atari (*.atr);;Todos los archivos (*)</translation>
    </message>
    <message>
        <location filename="../src/optionsdialog.cpp" line="606"/>
        <source>Select tool disk image</source>
        <translation>Seleccionar imagen de disco de herramienta</translation>
    </message>
    <message>
        <location filename="../src/optionsdialog.cpp" line="614"/>
        <source>Select Atari fixed width font</source>
        <translation>Seleccione la fuente de ancho fijo Atari</translation>
    </message>
    <message>
        <location filename="../src/optionsdialog.cpp" line="626"/>
        <source>Selec RCL image folder</source>
        <translation>Seleccione la carpeta de imágenes RCL</translation>
    </message>
    <message>
        <location filename="../src/optionsdialog.cpp" line="652"/>
        <source>Copy pics to AppData folder</source>
        <translation>Copiar imágenes a la carpeta AppData</translation>
    </message>
    <message>
        <location filename="../src/optionsdialog.cpp" line="659"/>
        <source>Copy pics to .respeqt_db subdir</source>
        <translation>Copiar imágenes al subdirectorio .respeqt_db</translation>
    </message>
    <message>
        <location filename="../src/optionsdialog.cpp" line="671"/>
        <source>Choose directory for app data</source>
        <translation>Elegir directorio para los datos de la aplicación</translation>
    </message>
</context>
<context>
    <name>OutputWindow</name>
    <message>
        <location filename="../ui/printers/outputwindow.ui" line="14"/>
        <source>RespeQt - Printer text output</source>
        <translation>RespeQt - Salida de texto por impresora</translation>
    </message>
    <message>
        <location filename="../ui/printers/outputwindow.ui" line="34"/>
        <location filename="../ui/printers/outputwindow.ui" line="37"/>
        <source>Atari Output (Graphics)</source>
        <translation>Salida Atari (gráficos)</translation>
    </message>
    <message>
        <location filename="../ui/printers/outputwindow.ui" line="52"/>
        <source>toolBar</source>
        <translation>Barra de Herramientas</translation>
    </message>
    <message>
        <location filename="../ui/printers/outputwindow.ui" line="82"/>
        <source>Save to a file...</source>
        <translation>Guardar en un archivo...</translation>
    </message>
    <message>
        <location filename="../ui/printers/outputwindow.ui" line="85"/>
        <source>Save contents to a file (Ctrl+S)</source>
        <translation>Guardar contenido en un archivo (Ctrl+S)</translation>
    </message>
    <message>
        <location filename="../ui/printers/outputwindow.ui" line="88"/>
        <source>Save contents to a file</source>
        <translation>Guardar contenido en un archivo</translation>
    </message>
    <message>
        <location filename="../ui/printers/outputwindow.ui" line="91"/>
        <location filename="../ui/printers/outputwindow.ui" line="278"/>
        <source>Ctrl+S</source>
        <translation></translation>
    </message>
    <message>
        <location filename="../ui/printers/outputwindow.ui" line="100"/>
        <source>Clear</source>
        <translation>Borrar</translation>
    </message>
    <message>
        <location filename="../ui/printers/outputwindow.ui" line="103"/>
        <source>Clear contents (Ctrl+C)</source>
        <translation>Borrar el contenido (Ctrl+C)</translation>
    </message>
    <message>
        <location filename="../ui/printers/outputwindow.ui" line="106"/>
        <source>Clear contents</source>
        <translation>Borrar el contenido</translation>
    </message>
    <message>
        <location filename="../ui/printers/outputwindow.ui" line="109"/>
        <source>Ctrl+C</source>
        <translation></translation>
    </message>
    <message>
        <location filename="../ui/printers/outputwindow.ui" line="124"/>
        <source>Word wrap</source>
        <translation>Ajuste de línea</translation>
    </message>
    <message>
        <location filename="../ui/printers/outputwindow.ui" line="127"/>
        <source>Toggle word wrapping (Ctrl+W)</source>
        <translation>Activar ajuste de texto (Ctrl+W)</translation>
    </message>
    <message>
        <location filename="../ui/printers/outputwindow.ui" line="130"/>
        <source>Toggle word wrapping</source>
        <translation>Activar ajuste de texto</translation>
    </message>
    <message>
        <location filename="../ui/printers/outputwindow.ui" line="133"/>
        <source>Ctrl+W</source>
        <translation></translation>
    </message>
    <message>
        <location filename="../ui/printers/outputwindow.ui" line="142"/>
        <source>Print</source>
        <translation>Imprimir</translation>
    </message>
    <message>
        <location filename="../ui/printers/outputwindow.ui" line="145"/>
        <source>Send contents to printer (Ctrl+P)</source>
        <translation>Enviar contenido a la impresora (Ctrl+P)</translation>
    </message>
    <message>
        <location filename="../ui/printers/outputwindow.ui" line="148"/>
        <source>Send contents to printer</source>
        <translation>Enviar contenido a la impresora</translation>
    </message>
    <message>
        <location filename="../ui/printers/outputwindow.ui" line="151"/>
        <source>Ctrl+P</source>
        <translation></translation>
    </message>
    <message>
        <location filename="../ui/printers/outputwindow.ui" line="161"/>
        <source>Atascii Font</source>
        <translation>Fuente Atascii</translation>
    </message>
    <message>
        <location filename="../ui/printers/outputwindow.ui" line="164"/>
        <source>Toggle ATASCII fonts (Alt+F)</source>
        <translation>Alternar las fuentes ATASCII (Alt+F)</translation>
    </message>
    <message>
        <location filename="../ui/printers/outputwindow.ui" line="167"/>
        <source>Toggle ATASCII fonts</source>
        <translation>Alternar las fuentes ATASCII</translation>
    </message>
    <message>
        <location filename="../ui/printers/outputwindow.ui" line="170"/>
        <source>Alt+F</source>
        <translation></translation>
    </message>
    <message>
        <location filename="../ui/printers/outputwindow.ui" line="186"/>
        <source>Font Size</source>
        <translation>Tamaño de la Fuente</translation>
    </message>
    <message>
        <location filename="../ui/printers/outputwindow.ui" line="189"/>
        <source>Toggle Font Size (6, 9, 12 pt) (Alt+Shift+F)</source>
        <translation>Cambiar tamaño de la fuente (6, 9, 12 pt) (Alt+Shift+F)</translation>
    </message>
    <message>
        <location filename="../ui/printers/outputwindow.ui" line="192"/>
        <source>Toggle Font Size (6, 9, 12 pt)</source>
        <translation>Cambiar tamaño de la fuente (6, 9, 12 pt)</translation>
    </message>
    <message>
        <location filename="../ui/printers/outputwindow.ui" line="195"/>
        <source>Alt+Shift+F</source>
        <translation></translation>
    </message>
    <message>
        <location filename="../ui/printers/outputwindow.ui" line="214"/>
        <source>Hide/Show Ascii</source>
        <translation>Mostrar/Ocultar Ascii</translation>
    </message>
    <message>
        <location filename="../ui/printers/outputwindow.ui" line="217"/>
        <source>Hide/Show Ascii Printer Output (Alt+Shift+H)</source>
        <translation>Ocultar/mostrar la salida por impresora Ascii (Alt+Shift+H)</translation>
    </message>
    <message>
        <location filename="../ui/printers/outputwindow.ui" line="220"/>
        <source>Hide/Show Ascii Printer Output</source>
        <translation>Ocultar/mostrar la salida por impresora Ascii</translation>
    </message>
    <message>
        <location filename="../ui/printers/outputwindow.ui" line="223"/>
        <source>Alt+Shift+H</source>
        <translation></translation>
    </message>
    <message>
        <location filename="../ui/printers/outputwindow.ui" line="245"/>
        <source>HideShow_Atascii</source>
        <translation>Mostrar/Ocultar Ascii</translation>
    </message>
    <message>
        <location filename="../ui/printers/outputwindow.ui" line="248"/>
        <source> Hide/Show Atascii Printer Output (Alt+H)</source>
        <translation> Ocultar/Mostrar salida de impresora Atascii (Alt+H)</translation>
    </message>
    <message>
        <location filename="../ui/printers/outputwindow.ui" line="251"/>
        <source>Hide/Show Atascii Printer Output</source>
        <translation>Ocultar/Mostrar la salida por impresora Ascii</translation>
    </message>
    <message>
        <location filename="../ui/printers/outputwindow.ui" line="254"/>
        <source>Alt+H</source>
        <translation></translation>
    </message>
    <message>
        <location filename="../ui/printers/outputwindow.ui" line="269"/>
        <source>Strip Line Numbers</source>
        <translation>Tira números de línea</translation>
    </message>
    <message>
        <location filename="../ui/printers/outputwindow.ui" line="272"/>
        <source>Strip Line numbers from the text output (Ctrl-S)</source>
        <translation>Tira números de línea de la salida de texto (Ctrl-S)</translation>
    </message>
    <message>
        <location filename="../ui/printers/outputwindow.ui" line="275"/>
        <source>Strip Line numbers from the text output</source>
        <translation>Eliminar números de línea de la salida de texto</translation>
    </message>
</context>
<context>
    <name>PCLINK</name>
    <message>
        <location filename="../src/pclink.cpp" line="175"/>
        <source>PCLINK Command=[$%1] aux1=$%2 aux2=$%3 cunit=$%4</source>
        <translation>PCLINK Comando=[$%1] aux1=$%2 aux2=$%3 cunit=$%4</translation>
    </message>
    <message>
        <location filename="../src/pclink.cpp" line="181"/>
        <source>[%1] P</source>
        <translation></translation>
    </message>
    <message>
        <location filename="../src/pclink.cpp" line="187"/>
        <source>[%1] R</source>
        <translation></translation>
    </message>
    <message>
        <location filename="../src/pclink.cpp" line="205"/>
        <source>[%1] Get status for [%2]</source>
        <translation>[%1] Obtener estado para [%2]</translation>
    </message>
    <message>
        <location filename="../src/pclink.cpp" line="216"/>
        <source>[%1] Speed poll</source>
        <translation>[%1] Encuesta de velocidad</translation>
    </message>
    <message>
        <location filename="../src/pclink.cpp" line="222"/>
        <source>[%1] command: $%2, aux: $%3 NAKed.</source>
        <translation>[%1] comando: $%2, aux: $%3 NAKed.</translation>
    </message>
    <message>
        <location filename="../src/pclink.cpp" line="250"/>
        <source>PCLINK[%1] Mount %2</source>
        <translation>PCLINK[%1] Montar %2</translation>
    </message>
    <message>
        <location filename="../src/pclink.cpp" line="286"/>
        <source>PCLINK[%1] Unmount</source>
        <translation>PCLINK[%1] Desmontar</translation>
    </message>
    <message>
        <location filename="../src/pclink.cpp" line="425"/>
        <source>match: %1%2%3%4%5%6%7%8%9%10%11 with %12%13%14%15%16%17%18%19%20%21%22: </source>
        <translation>Coincidencia: %1%2%3%4%5%6%7%8%9%10%11 con %12%13%14%15%16%17%18%19%20%21%22: </translation>
    </message>
    <message>
        <location filename="../src/pclink.cpp" line="432"/>
        <source>no match</source>
        <translation>Sin coincidencia</translation>
    </message>
    <message>
        <location filename="../src/pclink.cpp" line="442"/>
        <source>atr mismatch: not HIDDEN or ARCHIVED</source>
        <translation>Desajuste ATR: no OCULTO o ARCHIVADO</translation>
    </message>
    <message>
        <location filename="../src/pclink.cpp" line="448"/>
        <source>atr mismatch: not PROTECTED</source>
        <translation>No coincidencia ATR: no está PROTEGIDO</translation>
    </message>
    <message>
        <location filename="../src/pclink.cpp" line="455"/>
        <source>atr mismatch: not UNPROTECTED</source>
        <translation>Desajuste ATR: no DESPROTECTADO</translation>
    </message>
    <message>
        <location filename="../src/pclink.cpp" line="462"/>
        <source>atr mismatch: not SUBDIR</source>
        <translation>No coincidencia ATR: no SUBDIR</translation>
    </message>
    <message>
        <location filename="../src/pclink.cpp" line="469"/>
        <source>atr mismatch: not FILE</source>
        <translation>No coincidencia ATR: no archivo</translation>
    </message>
    <message>
        <location filename="../src/pclink.cpp" line="474"/>
        <source>match</source>
        <translation>Coincidencia</translation>
    </message>
    <message>
        <location filename="../src/pclink.cpp" line="535"/>
        <source>%1: got fname &apos;%2&apos;</source>
        <translation>%1: tiene nombre &apos;%2&apos;</translation>
    </message>
    <message>
        <location filename="../src/pclink.cpp" line="543"/>
        <source>%1: stat &apos;%2&apos;</source>
        <translation>%1: estado &apos;%2&apos;</translation>
    </message>
    <message>
        <location filename="../src/pclink.cpp" line="553"/>
        <source>&apos;%1&apos;: is a symlink</source>
        <translation>&apos;%1&apos;: es un enlace simbólico</translation>
    </message>
    <message>
        <location filename="../src/pclink.cpp" line="557"/>
        <source>&apos;%1&apos;: can&apos;t be accessed</source>
        <translation>&apos;%1&apos;: no se puede acceder</translation>
    </message>
    <message>
        <location filename="../src/pclink.cpp" line="558"/>
        <source>access error code %1</source>
        <translation>Código de error de acceso %1</translation>
    </message>
    <message>
        <location filename="../src/pclink.cpp" line="647"/>
        <source>Internal error: dir_cache should be nullptr!</source>
        <translation>Error interno: dir_cache debe ser NULL!</translation>
    </message>
    <message>
        <location filename="../src/pclink.cpp" line="769"/>
        <source>closing all files</source>
        <translation>Cerrando todos los archivos</translation>
    </message>
    <message>
        <location filename="../src/pclink.cpp" line="1010"/>
        <source>&apos;P&apos; WRONG DATA FRAME, expected size %1 got %2</source>
        <translation>&apos;P&apos; MARCO DE DATOS INCORRECTO, tamaño esperado %1 obtenido %2</translation>
    </message>
    <message>
        <location filename="../src/pclink.cpp" line="1023"/>
        <source>PARBLK retry, ignored</source>
        <translation>Reintento de PARBLK, ignorado</translation>
    </message>
    <message>
        <location filename="../src/pclink.cpp" line="1036"/>
        <source>%1 (fno $%02)</source>
        <translation></translation>
    </message>
    <message>
        <location filename="../src/pclink.cpp" line="1048"/>
        <source>bad handle 1 %1</source>
        <translation>Mal manejo 1 %1</translation>
    </message>
    <message>
        <location filename="../src/pclink.cpp" line="1054"/>
        <location filename="../src/pclink.cpp" line="1165"/>
        <source>bad size $0000 (0)</source>
        <translation>Mal tamaño $ 0000 (0)</translation>
    </message>
    <message>
        <location filename="../src/pclink.cpp" line="1073"/>
        <source>size $%1 (%2), buffer $%3 (%4)</source>
        <translation>tamaño $%1 (%2), buffer $%3 (%4)</translation>
    </message>
    <message>
        <location filename="../src/pclink.cpp" line="1080"/>
        <location filename="../src/pclink.cpp" line="1180"/>
        <location filename="../src/pclink.cpp" line="1320"/>
        <location filename="../src/pclink.cpp" line="1454"/>
        <source>serial communication error, abort</source>
        <translation>error de comunicación serie, abortar</translation>
    </message>
    <message>
        <location filename="../src/pclink.cpp" line="1086"/>
        <location filename="../src/pclink.cpp" line="1186"/>
        <location filename="../src/pclink.cpp" line="1334"/>
        <location filename="../src/pclink.cpp" line="1397"/>
        <source>handle %1</source>
        <translation>encargarse de %1</translation>
    </message>
    <message>
        <location filename="../src/pclink.cpp" line="1100"/>
        <source>FREAD: cannot read %1 bytes from dir</source>
        <translation>FREAD: no puede leer %1 bytes de dir</translation>
    </message>
    <message>
        <location filename="../src/pclink.cpp" line="1111"/>
        <source>FREAD: cannot seek to $%1 (%2)</source>
        <translation>FREAD: no se puede buscar a $%1 (%2)</translation>
    </message>
    <message>
        <location filename="../src/pclink.cpp" line="1117"/>
        <source>FREAD: cannot read %1 bytes from file</source>
        <translation>FREAD: no puede leer %1 bytes del archivo</translation>
    </message>
    <message>
        <location filename="../src/pclink.cpp" line="1141"/>
        <source>FREAD: send $%1 (%2), status $%3</source>
        <translation>FREAD: enviar $%1 (%2), estado $%3</translation>
    </message>
    <message>
        <location filename="../src/pclink.cpp" line="1159"/>
        <source>bad handle 2 %1</source>
        <translation>Mango malo 2 %1</translation>
    </message>
    <message>
        <location filename="../src/pclink.cpp" line="1173"/>
        <source>size $%1 (%2)</source>
        <translation>Tamaño $%1 (%2)</translation>
    </message>
    <message>
        <location filename="../src/pclink.cpp" line="1190"/>
        <source>FWRITE: cannot seek to $%1 (%2)</source>
        <translation>FWRITE: no se puede buscar %1 (%2)</translation>
    </message>
    <message>
        <location filename="../src/pclink.cpp" line="1202"/>
        <source>FWRITE: block CRC mismatch</source>
        <translation>FWRITE: bloque CRC no coinciden</translation>
    </message>
    <message>
        <location filename="../src/pclink.cpp" line="1221"/>
        <source>FWRITE: cannot write %1 bytes to file</source>
        <translation>FWRITE: no se pueden escribir %1 bytes en el archivo</translation>
    </message>
    <message>
        <location filename="../src/pclink.cpp" line="1232"/>
        <source>FWRITE: received $%1 (%2), status $%3</source>
        <translation>FWRITE: recibió $%1 (%2), estado $%3</translation>
    </message>
    <message>
        <location filename="../src/pclink.cpp" line="1243"/>
        <source>bad handle 3 %1</source>
        <translation>handle malo 3 %1</translation>
    </message>
    <message>
        <location filename="../src/pclink.cpp" line="1250"/>
        <location filename="../src/pclink.cpp" line="1387"/>
        <location filename="../src/pclink.cpp" line="1422"/>
        <location filename="../src/pclink.cpp" line="1710"/>
        <location filename="../src/pclink.cpp" line="1808"/>
        <location filename="../src/pclink.cpp" line="1879"/>
        <location filename="../src/pclink.cpp" line="1960"/>
        <location filename="../src/pclink.cpp" line="2025"/>
        <location filename="../src/pclink.cpp" line="2101"/>
        <location filename="../src/pclink.cpp" line="2287"/>
        <source>bad exec</source>
        <translation>Mala ejecución</translation>
    </message>
    <message>
        <location filename="../src/pclink.cpp" line="1257"/>
        <source>handle %1, newpos $%2 (%3)</source>
        <translation></translation>
    </message>
    <message>
        <location filename="../src/pclink.cpp" line="1278"/>
        <source>bad handle 4 %1</source>
        <translation>handle malo 4 %1</translation>
    </message>
    <message>
        <location filename="../src/pclink.cpp" line="1285"/>
        <location filename="../src/pclink.cpp" line="1314"/>
        <location filename="../src/pclink.cpp" line="2222"/>
        <source>device $%1</source>
        <translation>Dispositivo $%1</translation>
    </message>
    <message>
        <location filename="../src/pclink.cpp" line="1296"/>
        <source>handle %1, send $%2 (%3)</source>
        <translation>handle %1, enviar $%2 (%3)</translation>
    </message>
    <message>
        <location filename="../src/pclink.cpp" line="1329"/>
        <source>bad handle 5 %1</source>
        <translation>handle malo 5 %1</translation>
    </message>
    <message>
        <location filename="../src/pclink.cpp" line="1343"/>
        <source>eof_flg %1</source>
        <translation></translation>
    </message>
    <message>
        <location filename="../src/pclink.cpp" line="1360"/>
        <source>FNEXT: EOF</source>
        <translation></translation>
    </message>
    <message>
        <location filename="../src/pclink.cpp" line="1369"/>
        <source>FNEXT: status %1, send $%2 $%3%4 $%5%6%7 %8%9%10%11%12%13%14%15%16%17%18 %19-%20-%21 %22:%23:%24</source>
        <translation>FNEXT: estado %1, enviar $%2 $%3%4 $%5%6%7 %8%9%10%11%12%13%14%15%16%17%18 %19-%20-%21 %22:%23:%24</translation>
    </message>
    <message>
        <location filename="../src/pclink.cpp" line="1392"/>
        <source>bad handle 6 %1</source>
        <translation>handle malo 6 %1</translation>
    </message>
    <message>
        <location filename="../src/pclink.cpp" line="1437"/>
        <source>mode: $%1, atr1: $%2, atr2: $%3, path: &apos;%4&apos;, name: &apos;%5&apos;</source>
        <translation>Modo: $%1, atr1: $%2, atr2: $%3, ruta: &apos;%4&apos;, nombre: &apos;%5&apos;</translation>
    </message>
    <message>
        <location filename="../src/pclink.cpp" line="1465"/>
        <source>unsupported fmode ($%1)</source>
        <translation>Modo no compatible ($%1)</translation>
    </message>
    <message>
        <location filename="../src/pclink.cpp" line="1473"/>
        <source>invalid path 1 &apos;%1&apos;</source>
        <translation>Ruta no válida 1 &apos;%1&apos;</translation>
    </message>
    <message>
        <location filename="../src/pclink.cpp" line="1478"/>
        <location filename="../src/pclink.cpp" line="1820"/>
        <source>local path &apos;%1&apos;</source>
        <translation>Ruta de acceso local &apos;%1&apos;</translation>
    </message>
    <message>
        <location filename="../src/pclink.cpp" line="1485"/>
        <source>FOPEN: too many channels open</source>
        <translation>FOPEN: Demasiados canales abiertos</translation>
    </message>
    <message>
        <location filename="../src/pclink.cpp" line="1491"/>
        <location filename="../src/pclink.cpp" line="1571"/>
        <source>FOPEN: cannot stat &apos;%1&apos;</source>
        <translation>FOPEN: No se puede iniciar &apos;%1&apos;</translation>
    </message>
    <message>
        <location filename="../src/pclink.cpp" line="1502"/>
        <source> ! fmode &amp; 0x10</source>
        <translation></translation>
    </message>
    <message>
        <location filename="../src/pclink.cpp" line="1534"/>
        <source>FOPEN: file not found</source>
        <translation>FOPEN: Archivo no encontrado</translation>
    </message>
    <message>
        <location filename="../src/pclink.cpp" line="1542"/>
        <source>FOPEN: creating file</source>
        <translation>FOPEN: Creando archivo</translation>
    </message>
    <message>
        <location filename="../src/pclink.cpp" line="1548"/>
        <source>FOPEN: bad filename &apos;%1&apos;</source>
        <translation>FOPEN: Mal nombre de archivo &apos;%1&apos;</translation>
    </message>
    <message>
        <location filename="../src/pclink.cpp" line="1567"/>
        <source>FOPEN: full local path &apos;%1&apos;</source>
        <translation>FOPEN: Ruta de acceso local completa &apos;%1&apos;</translation>
    </message>
    <message>
        <location filename="../src/pclink.cpp" line="1578"/>
        <source>FOPEN: &apos;%1&apos; is read-only</source>
        <translation>FOPEN: &apos;%1&apos; es de solo lectura</translation>
    </message>
    <message>
        <location filename="../src/pclink.cpp" line="1603"/>
        <source>FOPEN: cannot open &apos;%1&apos;, %2 (%3)</source>
        <translation>FOPEN: No se puede abrir &apos;%1&apos;, %2 (%3)</translation>
    </message>
    <message>
        <location filename="../src/pclink.cpp" line="1641"/>
        <source>FOPEN: bad handle 7 %1</source>
        <translation>FOPEN: Mal handle 7 %1</translation>
    </message>
    <message>
        <location filename="../src/pclink.cpp" line="1649"/>
        <source>FOPEN: %1 handle %2</source>
        <translation>FOPEN: %1 encargarse de %2</translation>
    </message>
    <message>
        <location filename="../src/pclink.cpp" line="1660"/>
        <source>FOPEN: dir EOF?</source>
        <translation>FOPEN: Final de directorio?</translation>
    </message>
    <message>
        <location filename="../src/pclink.cpp" line="1688"/>
        <source>FOPEN: send %1, send $%2 $%3%4 $%5%6%7 %8%9%10%11%12%13%14%15%16%17%18 %19-%20-%21 %22:%23:%24</source>
        <translation>FOPEN: enviar %1, enviar $%2 $%3%4 $%5%6%7 %8%9%10%11%12%13%14%15%16%17%18 %19-%20-%21 %22:%23:%24</translation>
    </message>
    <message>
        <location filename="../src/pclink.cpp" line="1717"/>
        <source>invalid path 2 &apos;%1&apos;</source>
        <translation>Ruta inválida 2 &apos;%1&apos;</translation>
    </message>
    <message>
        <location filename="../src/pclink.cpp" line="1725"/>
        <location filename="../src/pclink.cpp" line="1825"/>
        <source>cannot open dir &apos;%1&apos;</source>
        <translation>No se puede abrir el directorio &apos;%1&apos;</translation>
    </message>
    <message>
        <location filename="../src/pclink.cpp" line="1730"/>
        <source>local path &apos;%1&apos;, fatr1 $%2</source>
        <translation>Ruta de acceso local &apos;%1&apos;, fatr1 $%2</translation>
    </message>
    <message>
        <location filename="../src/pclink.cpp" line="1777"/>
        <source>RENAME: renaming &apos;%1&apos; -&gt; &apos;%2&apos;</source>
        <translation>RENAME: Cambio de nombre &apos;%1&apos; -&gt; &apos;%2&apos;</translation>
    </message>
    <message>
        <location filename="../src/pclink.cpp" line="1780"/>
        <source>RENAME: &apos;%1&apos; already exists</source>
        <translation>RENAME: &apos;%1&apos; Ya existe</translation>
    </message>
    <message>
        <location filename="../src/pclink.cpp" line="1786"/>
        <source>RENAME: %1</source>
        <translation>RENAME: %1</translation>
    </message>
    <message>
        <location filename="../src/pclink.cpp" line="1815"/>
        <source>invalid path 3 &apos;%1&apos;</source>
        <translation>Ruta inválida 3 &apos;%1&apos;</translation>
    </message>
    <message>
        <location filename="../src/pclink.cpp" line="1854"/>
        <source>REMOVE: delete &apos;%1&apos;</source>
        <translation>REMOVE: Borrar &apos;%1&apos;</translation>
    </message>
    <message>
        <location filename="../src/pclink.cpp" line="1856"/>
        <source>REMOVE: cannot delete &apos;%1&apos;</source>
        <translation>REMOVE: No se puede borrar &apos;%1&apos;</translation>
    </message>
    <message>
        <location filename="../src/pclink.cpp" line="1884"/>
        <source>illegal fatr2 $%1</source>
        <translation>fatr2 ilegal $%1</translation>
    </message>
    <message>
        <location filename="../src/pclink.cpp" line="1892"/>
        <source>invalid path 4 &apos;%1&apos;</source>
        <translation>Ruta inválida 4 &apos;%1&apos;</translation>
    </message>
    <message>
        <location filename="../src/pclink.cpp" line="1897"/>
        <source>local path &apos;%1&apos;, fatr1 $%2 fatr2 $%3</source>
        <translation>Ruta de acceso local &apos;%1&apos;, fatr1 $%2 fatr2 $%3</translation>
    </message>
    <message>
        <location filename="../src/pclink.cpp" line="1902"/>
        <source>CHMOD: cannot open dir &apos;%1&apos;</source>
        <translation>CHMOD: No se puede abrir el directorio &apos;%1&apos;</translation>
    </message>
    <message>
        <location filename="../src/pclink.cpp" line="1931"/>
        <source>CHMOD: change atrs in &apos;%1&apos;</source>
        <translation>CHMOD: Cambiar permiso en &apos;%1&apos;</translation>
    </message>
    <message>
        <location filename="../src/pclink.cpp" line="1939"/>
        <source>CHMOD: failed on &apos;%1&apos;</source>
        <translation>CHMOD: falla en &apos;%1&apos;</translation>
    </message>
    <message>
        <location filename="../src/pclink.cpp" line="1967"/>
        <source>invalid path 5 &apos;%1&apos;</source>
        <translation>ruta inválida 5 apos;%1&apos;</translation>
    </message>
    <message>
        <location filename="../src/pclink.cpp" line="1976"/>
        <location filename="../src/pclink.cpp" line="2041"/>
        <source>bad dir name &apos;%1&apos;</source>
        <translation>Mal nombre de directorio &apos;%1&apos;</translation>
    </message>
    <message>
        <location filename="../src/pclink.cpp" line="1989"/>
        <source>making dir &apos;%1&apos;, time %2-%3-%4 %5:%6:%7</source>
        <translation>Haciendo directorio &apos;%1&apos;, fecha %2-%3-%4 %5:%6:%7</translation>
    </message>
    <message>
        <location filename="../src/pclink.cpp" line="1992"/>
        <source>MKDIR: &apos;%1&apos; already exists</source>
        <translation>MKDIR: &apos;%1&apos; Ya existe</translation>
    </message>
    <message>
        <location filename="../src/pclink.cpp" line="2003"/>
        <source>MKDIR: cannot make dir &apos;%1&apos;</source>
        <translation>MKDIR: No se puede hacer el directorio &apos;%1&apos;</translation>
    </message>
    <message>
        <location filename="../src/pclink.cpp" line="2032"/>
        <source>invalid path 6 &apos;%1&apos;</source>
        <translation>ruta no válida 6 &apos;%1&apos;</translation>
    </message>
    <message>
        <location filename="../src/pclink.cpp" line="2053"/>
        <source>cannot stat &apos;%1&apos;</source>
        <translation>No se puede empezar &apos;%1&apos;</translation>
    </message>
    <message>
        <location filename="../src/pclink.cpp" line="2060"/>
        <source>&apos;%1&apos; can&apos;t be accessed</source>
        <translation>&apos;%1&apos; No se puede acceder</translation>
    </message>
    <message>
        <location filename="../src/pclink.cpp" line="2067"/>
        <source>&apos;%1&apos; is not a directory</source>
        <translation>&apos;%1&apos; No es un directorio</translation>
    </message>
    <message>
        <location filename="../src/pclink.cpp" line="2073"/>
        <source>dir &apos;%1&apos; is write-protected</source>
        <translation>Directorio &apos;%1&apos; protegido contra escritura</translation>
    </message>
    <message>
        <location filename="../src/pclink.cpp" line="2078"/>
        <source>delete dir &apos;%1&apos;</source>
        <translation>Eliminar directorio &apos;%1&apos;</translation>
    </message>
    <message>
        <location filename="../src/pclink.cpp" line="2084"/>
        <source>RMDIR: cannot del &apos;%1&apos;, %2 (%3)</source>
        <translation>RMDIR: No se puede borrar &apos;%1&apos;, %2 (%3)</translation>
    </message>
    <message>
        <location filename="../src/pclink.cpp" line="2108"/>
        <source>invalid path 7 &apos;%1&apos;</source>
        <translation>Ruta no válida 7 &apos;%1&apos;</translation>
    </message>
    <message>
        <location filename="../src/pclink.cpp" line="2117"/>
        <source>cannot access &apos;%1&apos;, %2</source>
        <translation>No puede acceder apos;%1&apos;, %2</translation>
    </message>
    <message>
        <location filename="../src/pclink.cpp" line="2140"/>
        <source>new current dir &apos;%1&apos;</source>
        <translation>Nuevo directorio actual &apos;%1&apos;</translation>
    </message>
    <message>
        <location filename="../src/pclink.cpp" line="2157"/>
        <source>device $1</source>
        <translation>Dispositivo $1</translation>
    </message>
    <message>
        <location filename="../src/pclink.cpp" line="2176"/>
        <source>send &apos;%1&apos;</source>
        <translation>Enviando &apos;%1&apos;</translation>
    </message>
    <message>
        <location filename="../src/pclink.cpp" line="2236"/>
        <source>reading &apos;%1&apos;</source>
        <translation>Leyendo &apos;%1&apos;</translation>
    </message>
    <message>
        <location filename="../src/pclink.cpp" line="2267"/>
        <source>DFREE: send info (%1 bytes)</source>
        <translation>DFREE: Enviar información (%1 bytes)</translation>
    </message>
    <message>
        <location filename="../src/pclink.cpp" line="2294"/>
        <source>invalid name</source>
        <translation>Nombre inválido</translation>
    </message>
    <message>
        <location filename="../src/pclink.cpp" line="2303"/>
        <source>writing &apos;%1&apos;</source>
        <translation>Escritura &apos;%1&apos;</translation>
    </message>
    <message>
        <location filename="../src/pclink.cpp" line="2319"/>
        <source>CHVOL: %1</source>
        <translation></translation>
    </message>
    <message>
        <location filename="../src/pclink.cpp" line="2325"/>
        <source>fno $%1 not implemented</source>
        <translation>fno $%1 no se ha implementado</translation>
    </message>
</context>
<context>
    <name>PrinterWidget</name>
    <message>
        <location filename="../ui/printerwidget.ui" line="29"/>
        <source>Widget</source>
        <translation>Panel</translation>
    </message>
    <message>
        <location filename="../ui/printerwidget.ui" line="74"/>
        <source>1:</source>
        <translation></translation>
    </message>
    <message>
        <location filename="../ui/printerwidget.ui" line="155"/>
        <source>Disconnect</source>
        <translation>Desconectar</translation>
    </message>
    <message>
        <location filename="../ui/printerwidget.ui" line="158"/>
        <source>Disconnect Printer</source>
        <translation>Desconecte la impresora</translation>
    </message>
    <message>
        <location filename="../ui/printerwidget.ui" line="161"/>
        <source>Unmount D%1</source>
        <translation>Desmontar D%1</translation>
    </message>
    <message>
        <location filename="../ui/printerwidget.ui" line="172"/>
        <source>Connect</source>
        <translation>Conectar</translation>
    </message>
    <message>
        <location filename="../ui/printerwidget.ui" line="175"/>
        <source>Connect Printer</source>
        <translation>Conectar impresora</translation>
    </message>
    <message>
        <location filename="../src/printerwidget.cpp" line="42"/>
        <location filename="../src/printerwidget.cpp" line="72"/>
        <source>None</source>
        <translation>Ninguna</translation>
    </message>
    <message>
        <location filename="../src/printerwidget.cpp" line="96"/>
        <source>Printers</source>
        <translation>Impresoras</translation>
    </message>
    <message>
        <location filename="../src/printerwidget.cpp" line="96"/>
        <source>Please select a printer emulation.</source>
        <translation>Seleccione una emulación de impresora.</translation>
    </message>
</context>
<context>
    <name>Printers::Atari1020</name>
    <message>
        <location filename="../src/printers/atari1020.cpp" line="83"/>
        <source>[%1] Get status: $%2</source>
        <translation>[%1] Obtener el estado: $%2</translation>
    </message>
    <message>
        <location filename="../src/printers/atari1020.cpp" line="119"/>
        <source>[%1] Print: data frame failed</source>
        <translation>[%1] Imprimir: el marco de datos falló</translation>
    </message>
    <message>
        <location filename="../src/printers/atari1020.cpp" line="129"/>
        <source>[%1] Print (%2 chars)</source>
        <translation>[%1] Imprimir (%2 caracteres)</translation>
    </message>
    <message>
        <location filename="../src/printers/atari1020.cpp" line="139"/>
        <source>[%1] command: $%2, aux: $%3 NAKed.</source>
        <translation>[%1] commando: $%2, aux: $%3 NAKed.</translation>
    </message>
    <message>
        <location filename="../src/printers/atari1020.cpp" line="146"/>
        <source>[%1] ignored</source>
        <translation>[%1] ignorado</translation>
    </message>
    <message>
        <location filename="../src/printers/atari1020.cpp" line="247"/>
        <source>[%1] Escape character repeated</source>
        <translation>[%1] Carácter de escape repetido</translation>
    </message>
    <message>
        <location filename="../src/printers/atari1020.cpp" line="256"/>
        <source>[%1] Entering international mode</source>
        <translation>[%1] Entrar en modo internacional</translation>
    </message>
    <message>
        <location filename="../src/printers/atari1020.cpp" line="264"/>
        <source>[%1] Exiting international mode</source>
        <translation>[%1] Saliendo del modo internacional</translation>
    </message>
    <message>
        <location filename="../src/printers/atari1020.cpp" line="284"/>
        <source>[%1] Escape character not on start of line</source>
        <translation>[%1] El carácter de escape no está al principio de la línea</translation>
    </message>
    <message>
        <location filename="../src/printers/atari1020.cpp" line="293"/>
        <source>[%1] Enter Graphics mode</source>
        <translation>[%1] Entrar en el modo de gráficos</translation>
    </message>
    <message>
        <location filename="../src/printers/atari1020.cpp" line="303"/>
        <source>[%1] Switch to 40 columns</source>
        <translation>[%1] Cambiar a 40 columnas</translation>
    </message>
    <message>
        <location filename="../src/printers/atari1020.cpp" line="311"/>
        <source>[%1] Switch to 20 columns</source>
        <translation>[%1] Cambiar a 20 columnas</translation>
    </message>
    <message>
        <location filename="../src/printers/atari1020.cpp" line="319"/>
        <source>[%1] Switch to 80 columns</source>
        <translation>[%1] Cambiar a 80 columnas</translation>
    </message>
    <message>
        <location filename="../src/printers/atari1020.cpp" line="328"/>
        <source>[%1] Enter international mode</source>
        <translation>[%1] Entrar en modo internacional</translation>
    </message>
    <message>
        <location filename="../src/printers/atari1020.cpp" line="337"/>
        <source>[%1] Exit international mode</source>
        <translation>[%1] Salir del modo internacional</translation>
    </message>
    <message>
        <location filename="../src/printers/atari1020.cpp" line="345"/>
        <source>[%1] Unknown control code $%2</source>
        <translation>[%1] Código de control desconocido $%2</translation>
    </message>
    <message>
        <location filename="../src/printers/atari1020.cpp" line="426"/>
        <source>[%1] Unknown Graphics command $%2</source>
        <translation>[%1] Comando de gráficos desconocido $%2</translation>
    </message>
    <message>
        <location filename="../src/printers/atari1020.cpp" line="563"/>
        <source>[%1] Exit Graphics mode</source>
        <translation>[%1] Salir del modo de gráficos</translation>
    </message>
    <message>
        <location filename="../src/printers/atari1020.cpp" line="570"/>
        <source>[%1] Move to Home</source>
        <translation>[%1] Mover al inicio</translation>
    </message>
    <message>
        <location filename="../src/printers/atari1020.cpp" line="585"/>
        <source>[%1] Scale characters to %2</source>
        <translation>[%1] Escalar caracteres a%2</translation>
    </message>
    <message>
        <location filename="../src/printers/atari1020.cpp" line="591"/>
        <source>[%1] Scale command ignored (%2 should be in range 0-63)</source>
        <translation>[%1] Comando de escala ignorado (%2 debe estar en el rango 0-63)</translation>
    </message>
    <message>
        <location filename="../src/printers/atari1020.cpp" line="624"/>
        <source>[%1] Set color to %2</source>
        <translation>[%1] Establecer el color en %2</translation>
    </message>
    <message>
        <location filename="../src/printers/atari1020.cpp" line="630"/>
        <source>[%1] Set color command ignored (%2 should be in range 0-3)</source>
        <translation>[%1] Se ignora el comando establecer color (%2 debe estar en el rango 0-3)</translation>
    </message>
    <message>
        <location filename="../src/printers/atari1020.cpp" line="643"/>
        <source>[%1] Set line mode to %2</source>
        <translation>[%1] Establecer el modo de línea en %2</translation>
    </message>
    <message>
        <location filename="../src/printers/atari1020.cpp" line="645"/>
        <source>solid</source>
        <translation>sólida</translation>
    </message>
    <message>
        <location filename="../src/printers/atari1020.cpp" line="645"/>
        <source>dashed %1</source>
        <translation>punteado %1</translation>
    </message>
    <message>
        <location filename="../src/printers/atari1020.cpp" line="659"/>
        <source>[%1] Set line mode command ignored (%2 should be in range 0-15)</source>
        <translation>[%1] Se ignora el comando de modo de línea de configuración (%2 debe estar en el rango 0-15)</translation>
    </message>
    <message>
        <location filename="../src/printers/atari1020.cpp" line="668"/>
        <source>[%1] Initialize plotter</source>
        <translation>[%1] Inicializar trazador</translation>
    </message>
    <message>
        <location filename="../src/printers/atari1020.cpp" line="697"/>
        <source>[%1] Draw to point (%2,%3)</source>
        <translation>[%1] Dibujar al punto (%2,%3)</translation>
    </message>
    <message>
        <location filename="../src/printers/atari1020.cpp" line="711"/>
        <source>[%1] Draw relative to point (%2,%3)</source>
        <translation>[%1] Dibujar en relación con el punto (%2,%3)</translation>
    </message>
    <message>
        <location filename="../src/printers/atari1020.cpp" line="722"/>
        <source>[%1] Move to point (%2,%3)</source>
        <translation>[%1] Mover al punto (%2,%3)</translation>
    </message>
    <message>
        <location filename="../src/printers/atari1020.cpp" line="733"/>
        <source>[%1] Move relative to point (%2,%3)</source>
        <translation>[%1] Mover en relación con el punto (%2,%3)</translation>
    </message>
    <message>
        <location filename="../src/printers/atari1020.cpp" line="753"/>
        <source>[%1] Draw X-axis with size %2 and %3 marks</source>
        <translation>[%1] Dibujar el eje X con marcas de tamaño %2 y %3</translation>
    </message>
    <message>
        <location filename="../src/printers/atari1020.cpp" line="761"/>
        <source>[%1] Draw Y-axis with size %2 and %3 marks</source>
        <translation>[%1] Dibujar el eje Y con marcas de tamaño %2 y %3</translation>
    </message>
    <message>
        <location filename="../src/printers/atari1020.cpp" line="778"/>
        <source>[%1] Set text orientation to %2°</source>
        <translation>[%1] Establecer la orientación del texto en %2°</translation>
    </message>
    <message>
        <location filename="../src/printers/atari1020.cpp" line="785"/>
        <source>[%1] Set text orientation command ignored (%2 should be in range 0-3)</source>
        <translation>[%1] Se ignora el comando de configuración de la orientación del texto (%2 debe estar en el rango 0-3)</translation>
    </message>
    <message>
        <location filename="../src/printers/atari1020.cpp" line="796"/>
        <source>[%1] Print &apos;%2&apos; in Graphics mode</source>
        <translation>[%1] Impresión &apos;%2&apos; en modo Gráficos</translation>
    </message>
</context>
<context>
    <name>Printers::BasePrinter</name>
    <message>
        <location filename="../src/printers/baseprinter.cpp" line="46"/>
        <source>[%1] Get status: $%2</source>
        <translation>[%1] Obtener estado: $%2</translation>
    </message>
    <message>
        <location filename="../src/printers/baseprinter.cpp" line="69"/>
        <source>[%1] Command: $%2, aux1: $%3, aux2: $%4 NAKed because aux1 is not supported</source>
        <translation>[%1] Comando: $%2, aux: $%3 NAKed porque aux2 no es compatible</translation>
    </message>
    <message>
        <location filename="../src/printers/baseprinter.cpp" line="78"/>
        <source>[%1] Print: data frame failed</source>
        <translation>[%1] Impresión: error en el marco de datos</translation>
    </message>
    <message>
        <location filename="../src/printers/baseprinter.cpp" line="87"/>
        <source>[%1] Print (%2 chars)</source>
        <translation>[%1] Imprimir (%2 caracteres)</translation>
    </message>
    <message>
        <location filename="../src/printers/baseprinter.cpp" line="93"/>
        <source>[%1] command: $%2, aux1: $%3, aux2: $%4 NAKed.</source>
        <translation>[%1] comando: $%2, aux1: $%3, aux2: $%4 NAKed.</translation>
    </message>
    <message>
        <location filename="../src/printers/baseprinter.cpp" line="96"/>
        <source>[%1] ignored</source>
        <translation>[%1] ignorado</translation>
    </message>
    <message>
        <location filename="../src/printers/baseprinter.cpp" line="120"/>
        <source>[%1] Receiving %2 bytes from Atari</source>
        <translation>[%1] Recibiendo %2 bytes de Atari</translation>
    </message>
    <message>
        <location filename="../src/printers/baseprinter.cpp" line="128"/>
        <source>[%1] Sending %2 bytes to Atari</source>
        <translation>[%1] Enviando %2 bytes a Atari</translation>
    </message>
    <message>
        <location filename="../src/printers/baseprinter.cpp" line="139"/>
        <source>[%1] §%2</source>
        <translation></translation>
    </message>
</context>
<context>
    <name>Printers::OutputWindow</name>
    <message>
        <location filename="../src/printers/outputwindow.cpp" line="123"/>
        <source>Save printer text output</source>
        <translation>Guardar texto impresora salida</translation>
    </message>
    <message>
        <location filename="../src/printers/outputwindow.cpp" line="125"/>
        <source>SVG files (*.svg);;All files (*)</source>
        <translation>Archivos SVG (*.svg);;Todos los archivos (*)</translation>
    </message>
</context>
<context>
    <name>Printers::Passthrough</name>
    <message>
        <location filename="../include/printers/passthrough.h" line="17"/>
        <source>Passthrough</source>
        <translation>Pasar por</translation>
    </message>
</context>
<context>
    <name>Printers::TextPrinterWindow</name>
    <message>
        <location filename="../src/printers/textprinterwindow.cpp" line="228"/>
        <source>Save printer text output</source>
        <translation>Guardar texto impresora salida</translation>
    </message>
    <message>
        <location filename="../src/printers/textprinterwindow.cpp" line="229"/>
        <source>Text files (*.txt);;All files (*)</source>
        <translation>Archivos de texto (* txt.);;Todos los archivos (*)</translation>
    </message>
    <message>
        <location filename="../src/printers/textprinterwindow.cpp" line="260"/>
        <source>Stripping Line Numbers..</source>
        <translation>Excluyendo números de línea..</translation>
    </message>
    <message>
        <location filename="../src/printers/textprinterwindow.cpp" line="260"/>
        <source>The text does not seem to contain any line numbers!</source>
        <translation>El texto no parece contener ningún número de línea!</translation>
    </message>
</context>
<context>
    <name>QObject</name>
    <message>
        <location filename="../include/printers/rawoutput.h" line="30"/>
        <source>Raw output</source>
        <translation>Salida sin procesar</translation>
    </message>
    <message>
        <location filename="include/printers/textprinterwindow.h" line="49"/>
        <source>Text printer</source>
        <translation></translation>
    </message>
    <message>
        <location filename="../src/printers/rawoutput_cups.cpp" line="115"/>
        <location filename="../src/printers/rawoutput_win.cpp" line="81"/>
        <source>Select raw printer</source>
        <translation>Seleccionar impresora sin formato</translation>
    </message>
</context>
<context>
    <name>RCl</name>
    <message>
        <location filename="../src/rcl.cpp" line="48"/>
        <location filename="../src/rcl.cpp" line="261"/>
        <location filename="../src/rcl.cpp" line="483"/>
        <source>[%1] Read data frame failed</source>
        <translation>[%1] Error al leer el marco de datos</translation>
    </message>
    <message>
        <location filename="../src/rcl.cpp" line="57"/>
        <source>[%1] List filter set: [%2]</source>
        <translation>[%1] Conjunto de filtros de lista: [%2]</translation>
    </message>
    <message>
        <location filename="../src/rcl.cpp" line="151"/>
        <source>[%1] Date/time sent to client (%2).</source>
        <translation>[%1] Fecha/hora enviada al cliente (%2).</translation>
    </message>
    <message>
        <location filename="../src/rcl.cpp" line="172"/>
        <source>[%1] Swapped disk %2 with disk %3.</source>
        <translation>[%1] Disco intercambiado %2 con disco %3.</translation>
    </message>
    <message>
        <location filename="../src/rcl.cpp" line="176"/>
        <source>[%1] Invalid swap request for drives: (%2)-(%3).</source>
        <translation>[%1] Solicitud de intercambio no válida para unidades: (%2)-(%3).</translation>
    </message>
    <message>
        <location filename="../src/rcl.cpp" line="204"/>
        <source>[%1] Unmounted disk %2</source>
        <translation>[%1] Disco sin montar %2</translation>
    </message>
    <message>
        <location filename="../src/rcl.cpp" line="206"/>
        <source>[%1] ALL images were remotely unmounted</source>
        <translation>[%1] TODAS las imágenes fueron desmontadas remotamente</translation>
    </message>
    <message>
        <location filename="../src/rcl.cpp" line="210"/>
        <source>[%1] Can not remotely unmount ALL images due to pending changes.</source>
        <translation>[%1] No se puede desmontar remotamente TODAS las imágenes debido a cambios pendientes.</translation>
    </message>
    <message>
        <location filename="../src/rcl.cpp" line="218"/>
        <source>[%1] Can not remotely unmount disk %2 due to pending changes.</source>
        <translation>[%1] No se puede desmontar remotamente el disco %2 debido a cambios pendientes.</translation>
    </message>
    <message>
        <location filename="../src/rcl.cpp" line="223"/>
        <source>[%1] Remotely unmounted disk %2</source>
        <translation>[%1] Disco desmontado remotamente %2</translation>
    </message>
    <message>
        <location filename="../src/rcl.cpp" line="229"/>
        <location filename="../src/rcl.cpp" line="456"/>
        <source>[%1] Invalid drive number: %2 for remote unmount</source>
        <translation>[%1] Número de unidad no válido: %2 para desmontaje remoto</translation>
    </message>
    <message>
        <location filename="../src/rcl.cpp" line="243"/>
        <location filename="../src/rcl.cpp" line="471"/>
        <source>[%1] RespeQt can&apos;t determine the folder where the image file must be created/mounted!</source>
        <translation>[%1] RespeQt no puede determinar la carpeta donde se debe crear/montar el archivo de imagen!</translation>
    </message>
    <message>
        <location filename="../src/rcl.cpp" line="244"/>
        <location filename="../src/rcl.cpp" line="472"/>
        <source>[%1] Mount a Folder Image at least once before issuing a remote mount command.</source>
        <translation>[%1] Monte una imagen de carpeta al menos una vez antes de emitir un comando de montaje remoto.</translation>
    </message>
    <message>
        <location filename="../src/rcl.cpp" line="278"/>
        <source>[%1] Invalid image file attribute: %2</source>
        <translation>[%1] Atributo de archivo de imagen no válido: %2</translation>
    </message>
    <message>
        <location filename="../src/rcl.cpp" line="286"/>
        <source>[%1] Can not create PC File: %2</source>
        <translation>[%1] No se puede crear un archivo de PC: %2</translation>
    </message>
    <message>
        <location filename="../src/rcl.cpp" line="436"/>
        <location filename="../src/rcl.cpp" line="447"/>
        <source>[%1] Saved disk %2</source>
        <translation>[%1] Disco guardado %2</translation>
    </message>
    <message>
        <location filename="../src/rcl.cpp" line="563"/>
        <source>[%1] command: $%2, aux1: $%3, aux2: $%4 NAKed.</source>
        <translation>[%1] commando: $%2, aux1: $%3, aux2: $%4 NAKed</translation>
    </message>
    <message>
        <location filename="../src/rcl.cpp" line="580"/>
        <source>[%1] Image %2 mounted</source>
        <translation>[%1] Imagen %2 montada</translation>
    </message>
</context>
<context>
    <name>SioRecorder</name>
    <message>
        <location filename="../src/siorecorder.cpp" line="150"/>
        <source>Sleeping %1 milliseconds</source>
        <translation>Durmiendo %1 milisegundos</translation>
    </message>
    <message>
        <location filename="../src/siorecorder.cpp" line="215"/>
        <source>Read data frame with size %1</source>
        <translation>Leer marco de datos con tamaño %1</translation>
    </message>
</context>
<context>
    <name>SioWorker</name>
    <message>
        <location filename="../src/sioworker.cpp" line="127"/>
        <source>Cannot read command frame.</source>
        <translation>No se puede leer trama de comando.</translation>
    </message>
    <message>
        <location filename="../src/sioworker.cpp" line="129"/>
        <source>Trying to reconnect SIO port...</source>
        <translation>Intentando volver a conectar el puerto SIO...</translation>
    </message>
    <message>
        <location filename="../src/sioworker.cpp" line="154"/>
        <source>[%1] command: $%2, aux: $%3 ignored because the image explorer is open.</source>
        <translation>[%1] comando: $%2, aux: $%3  ignorado porque el explorador de imagen está abierta.</translation>
    </message>
    <message>
        <location filename="../src/sioworker.cpp" line="158"/>
        <source>[%1] command: $%2, aux: $%3 ignored: %4</source>
        <translation>[%1] comando: $%2, aux: $%3 ignorado: %4</translation>
    </message>
    <message>
        <location filename="../src/sioworker.cpp" line="160"/>
        <source>[%1] command: $%2, aux1: $%3, aux2: $%4 ignored.</source>
        <translation>[%1] commando: $%2, aux1: $%3, aux2: $%4 ignorado.</translation>
    </message>
    <message>
        <location filename="../src/sioworker.cpp" line="174"/>
        <source>[810 Rev. E] Upload and Execute Code or [Speedy 1050] Format Disk Asynchronously</source>
        <translation>[810 Rev. E] Cargar y ejecutar código o [Speedy 1050] Formatear disco de forma asincrónica</translation>
    </message>
    <message>
        <location filename="../src/sioworker.cpp" line="176"/>
        <location filename="../src/sioworker.cpp" line="293"/>
        <source>Format Single Density Disk</source>
        <translation>Formatear disco de densidad única</translation>
    </message>
    <message>
        <location filename="../src/sioworker.cpp" line="178"/>
        <location filename="../src/sioworker.cpp" line="295"/>
        <source>Format Enhanced Density Disk</source>
        <translation>Formatear disco de densidad mejorada</translation>
    </message>
    <message>
        <location filename="../src/sioworker.cpp" line="180"/>
        <source>[1050, Happy 1050] Run Speed Diagnostic or [Turbo 1050] Service Put</source>
        <translation>[1050, Happy 1050] Ejecutar diagnóstico de velocidad o [Turbo 1050] Puesta en servicio</translation>
    </message>
    <message>
        <location filename="../src/sioworker.cpp" line="182"/>
        <source>[1050, Happy 1050] Run Diagnostic or [Turbo 1050] Service Get</source>
        <translation>[1050, Happy 1050] Ejecutar diagnóstico o [Turbo 1050] Obtener servicio</translation>
    </message>
    <message>
        <location filename="../src/sioworker.cpp" line="184"/>
        <source>[Happy 1050 Rev. 7] Clear Sector Flags (Broadcast)</source>
        <translation>[Happy 1050 Rev. 7] Borrar las banderas del sector (transmisión)</translation>
    </message>
    <message>
        <location filename="../src/sioworker.cpp" line="186"/>
        <source>[Happy 1050 Rev. 7] Get Sector Flags (Broadcast)</source>
        <translation>[Happy 1050 Rev. 7] Obtener indicadores de sector (transmisión)</translation>
    </message>
    <message>
        <location filename="../src/sioworker.cpp" line="188"/>
        <source>[Happy 1050 Rev. 7] Send Sector (Broadcast)</source>
        <translation>[Happy 1050 Rev. 7] Sector de envío (transmisión)</translation>
    </message>
    <message>
        <location filename="../src/sioworker.cpp" line="190"/>
        <source>[Super Archiver 1050, Happy 1050, Speedy 1050, Duplicator 1050] Poll Speed</source>
        <translation>[Super Archiver 1050, Happy 1050, Speedy 1050, Duplicator 1050] Velocidad de encuesta</translation>
    </message>
    <message>
        <location filename="../src/sioworker.cpp" line="192"/>
        <source>[Happy 810/1050 Rev. 7] Read Track or [Speedy 1050] Add or Delete a Command</source>
        <translation>[Happy 810/1050 Rev. 7] Leer pista o [Speedy 1050] Agregar o eliminar un comando</translation>
    </message>
    <message>
        <location filename="../src/sioworker.cpp" line="194"/>
        <source>[Chip 810, Super Archiver 1050] Write Sector using Index or [Happy 810/1050 Rev. 7] Read All Sectors</source>
        <translation>[Chip 810, Super Archiver 1050] Escribir sector usando índice o [Happy 810/1050 Rev. 7] Leer todos los sectores</translation>
    </message>
    <message>
        <location filename="../src/sioworker.cpp" line="196"/>
        <source>[Chip 810, Super Archiver 1050] Read All Sector Statuses or [Happy 810/1050 Rev. 7] Set Skew Alignment</source>
        <translation>[Chip 810, Super Archiver 1050] Leer todos los estados de sector o [Happy 810/1050 Rev. 7] Establecer alineación sesgada</translation>
    </message>
    <message>
        <location filename="../src/sioworker.cpp" line="198"/>
        <source>[Chip 810, Super Archiver 1050] Read Sector using Index or [Happy 810/1050 Rev. 7] Read Skew Alignment or [Speedy 1050] Configure Drive and Display</source>
        <translation>[Chip 810, Super Archiver 1050] Leer sector usando índice o [Happy 810/1050 Rev. 7] Leer alineación sesgada o [Speedy 1050] Configurar unidad y visualización</translation>
    </message>
    <message>
        <location filename="../src/sioworker.cpp" line="200"/>
        <source>[Chip 810, Super Archiver 1050] Write Track</source>
        <translation>[Chip 810, Super Archiver 1050] Pista de escritura</translation>
    </message>
    <message>
        <location filename="../src/sioworker.cpp" line="202"/>
        <source>[Chip 810, Super Archiver 1050] Read Track (128 bytes)</source>
        <translation>[Chip 810, Super Archiver 1050] Leer pista (128 bytes)</translation>
    </message>
    <message>
        <location filename="../src/sioworker.cpp" line="205"/>
        <source>[Happy 810/1050 Rev. 7] Set Idle Timeout</source>
        <translation>[Happy 810/1050 Rev. 7] Establecer tiempo de espera inactivo</translation>
    </message>
    <message>
        <location filename="../src/sioworker.cpp" line="207"/>
        <source>[Happy 810/1050 Rev. 7] Set Alternate Device ID</source>
        <translation>[Happy 810/1050 Rev. 7] Establecer ID de dispositivo alternativo</translation>
    </message>
    <message>
        <location filename="../src/sioworker.cpp" line="209"/>
        <source>[Happy 810/1050 Rev. 7] Reinitialize Drive</source>
        <translation>[Happy 810/1050 Rev. 7] Reinicializar Drive</translation>
    </message>
    <message>
        <location filename="../src/sioworker.cpp" line="211"/>
        <source>[Happy 810/1050 Rev. 7] Configure Drive</source>
        <translation>[Happy 810/1050 Rev. 7] Configurar unidad</translation>
    </message>
    <message>
        <location filename="../src/sioworker.cpp" line="213"/>
        <source>[Happy 810/1050 Rev. 7] Write Track with Skew Alignment</source>
        <translation>[Happy 810/1050 Rev. 7] Escribir pista con alineación sesgada</translation>
    </message>
    <message>
        <location filename="../src/sioworker.cpp" line="215"/>
        <source>[Happy 810/1050 Rev. 7] Init Skew Alignment</source>
        <translation>[Happy 810/1050 Rev. 7] Alineación sesgada inicial</translation>
    </message>
    <message>
        <location filename="../src/sioworker.cpp" line="217"/>
        <source>[Happy 810/1050 Rev. 7] Prepare backup or [Speedy 1050] Configure Slow/Fast Speed</source>
        <translation>[Happy 810/1050 Rev. 7] Prepare una copia de seguridad o [Speedy 1050] Configure la velocidad lenta/rápida</translation>
    </message>
    <message>
        <location filename="../src/sioworker.cpp" line="219"/>
        <source>[Chip 810, Super Archiver 1050] Set RAM Buffer or [Speedy 1050] Jump to Address</source>
        <translation>[Chip 810, Super Archiver 1050] Establecer buffer RAM o [Speedy 1050] Saltar a dirección</translation>
    </message>
    <message>
        <location filename="../src/sioworker.cpp" line="221"/>
        <source>[Chip 810] Upload and Execute Code or [Speedy 1050] Jump to Address with Acknowledge</source>
        <translation>[Chip 810] Cargar y ejecutar código o [Speedy 1050] Saltar a la dirección con reconocimiento</translation>
    </message>
    <message>
        <location filename="../src/sioworker.cpp" line="223"/>
        <source>[Super Archiver 1050, Speedy 1050, Turbo 1050, Duplicator 1050] Get PERCOM Block or [Chip 810] Set Shutdown Delay</source>
        <translation>[Super Archiver 1050, Speedy 1050, Turbo 1050, Duplicator 1050] Obtener bloque PERCOM o [Chip 810] Establecer retardo de apagado</translation>
    </message>
    <message>
        <location filename="../src/sioworker.cpp" line="225"/>
        <source>[Speedy 1050, Turbo 1050, Duplicator 1050] Set PERCOM Block or [Chip 810, Super Archiver 1050] Open CHIP</source>
        <translation>[Speedy 1050, Turbo 1050, Duplicator 1050] Establecer bloque PERCOM o [Chip 810, Super Archiver 1050] Abrir CHIP</translation>
    </message>
    <message>
        <location filename="../src/sioworker.cpp" line="227"/>
        <location filename="../src/sioworker.cpp" line="344"/>
        <source>Put Sector (no verify)</source>
        <translation>Poner sector (no verificar)</translation>
    </message>
    <message>
        <location filename="../src/sioworker.cpp" line="229"/>
        <source>[Happy 810] Execute code or [Speedy 1050] Flush Write</source>
        <translation>[Happy 810] Ejecutar código o [Speedy 1050] Escritura al ras</translation>
    </message>
    <message>
        <location filename="../src/sioworker.cpp" line="231"/>
        <location filename="../src/sioworker.cpp" line="348"/>
        <source>Read Sector</source>
        <translation>Leer sector</translation>
    </message>
    <message>
        <location filename="../src/sioworker.cpp" line="233"/>
        <location filename="../src/sioworker.cpp" line="350"/>
        <source>Get Status</source>
        <translation>Obtener el estado</translation>
    </message>
    <message>
        <location filename="../src/sioworker.cpp" line="235"/>
        <source>[Chip 810, Super Archiver 1050, 810 Rev. E] Get RAM Buffer</source>
        <translation>[Chip 810, Super Archiver 1050, 810 Rev. E] Obtener buffer RAM</translation>
    </message>
    <message>
        <location filename="../src/sioworker.cpp" line="237"/>
        <location filename="../src/sioworker.cpp" line="239"/>
        <source>[Happy 810] Execute code</source>
        <translation>[Happy 810] Ejecutar código</translation>
    </message>
    <message>
        <location filename="../src/sioworker.cpp" line="241"/>
        <location filename="../src/sioworker.cpp" line="358"/>
        <source>Write Sector (with verify)</source>
        <translation>Escribir sector (con verificar)</translation>
    </message>
    <message>
        <location filename="../src/sioworker.cpp" line="243"/>
        <source>[Chip 810, Super Archiver 1050] Set Trace On/Off</source>
        <translation>[Chip 810, Super Archiver 1050] Activar/desactivar el seguimiento</translation>
    </message>
    <message>
        <location filename="../src/sioworker.cpp" line="245"/>
        <source>[Speedy 1050] Write Track at Address</source>
        <translation>[Speedy 1050] Escribir pista en la dirección</translation>
    </message>
    <message>
        <location filename="../src/sioworker.cpp" line="247"/>
        <source>[Duplicator 1050] Analyze Track</source>
        <translation>[Duplicator 1050] Analizar pista</translation>
    </message>
    <message>
        <location filename="../src/sioworker.cpp" line="249"/>
        <source>[Super Archiver 1050] Write Fuzzy Sector using Index or [Speedy 1050] Read Track at Address or [Duplicator 1050] Set Drive buffer Mode</source>
        <translation>[Super Archiver 1050] Escribir sector difuso usando índice o [Speedy 1050] Leer pista en dirección o [Duplicador 1050] Establecer modo de buffer de unidad</translation>
    </message>
    <message>
        <location filename="../src/sioworker.cpp" line="251"/>
        <source>[Duplicator 1050] Custom Track Format</source>
        <translation>[Duplicator 1050] Formato de pista personalizado</translation>
    </message>
    <message>
        <location filename="../src/sioworker.cpp" line="253"/>
        <source>[Duplicator 1050] Set Density Sensing</source>
        <translation>[Duplicator 1050] Establecer detección de densidad</translation>
    </message>
    <message>
        <location filename="../src/sioworker.cpp" line="255"/>
        <source>[Super Archiver 1050, Duplicator 1050] Format with Custom Sector Skewing</source>
        <translation>[Super Archiver 1050, Duplicator 1050] Formato con sesgo de sector personalizado</translation>
    </message>
    <message>
        <location filename="../src/sioworker.cpp" line="257"/>
        <source>[Super Archiver 1050] Read Track (256 bytes) or [Duplicator 1050] Seek to Track</source>
        <translation>[Super Archiver 1050] Leer pista (256 bytes) o [Duplicador 1050] Buscar pista</translation>
    </message>
    <message>
        <location filename="../src/sioworker.cpp" line="259"/>
        <source>[Speedy 1050] Get SIO Routine size or [Duplicator 1050] Change Drive Hold on Time</source>
        <translation>[Speedy 1050] Obtener el tamaño de la rutina SIO o [Duplicator 1050] Cambiar el tiempo de espera de la unidad</translation>
    </message>
    <message>
        <location filename="../src/sioworker.cpp" line="261"/>
        <source>[Speedy 1050] Get SIO Routine relocated at Address</source>
        <translation>[Speedy 1050] Reubicar la rutina SIO en la dirección</translation>
    </message>
    <message>
        <location filename="../src/sioworker.cpp" line="263"/>
        <source>[Duplicator 1050] Change Drive RPM</source>
        <translation>[Duplicator 1050] Cambiar las RPM de la unidad</translation>
    </message>
    <message>
        <location filename="../src/sioworker.cpp" line="265"/>
        <source>[Duplicator 1050] Upload Sector Pattern and Read Track</source>
        <translation>[Duplicator 1050] Cargar patrón de sector y leer pista</translation>
    </message>
    <message>
        <location filename="../src/sioworker.cpp" line="267"/>
        <source>[Duplicator 1050] Upload Sector Pattern and Write Buffer to Disk</source>
        <translation>[Duplicator 1050] Cargar patrón de sector y escribir buffer en disco</translation>
    </message>
    <message>
        <location filename="../src/sioworker.cpp" line="269"/>
        <source>[Happy 810/1050 Rev. 7] High Speed Put Sector (no verify)</source>
        <translation>[Happy 810/1050 Rev. 7] Sector Put de alta velocidad (sin verificación)</translation>
    </message>
    <message>
        <location filename="../src/sioworker.cpp" line="271"/>
        <source>[Super Archiver 1050] Write Fuzzy Sector</source>
        <translation>[Super Archiver 1050] Escribir sector difuso</translation>
    </message>
    <message>
        <location filename="../src/sioworker.cpp" line="273"/>
        <source>[Happy 810/1050 Rev. 7] High Speed Read Sector or [Duplicator 1050] Run Uploaded Program at Address</source>
        <translation>[Happy 810/1050 Rev. 7] Sector de lectura de alta velocidad o [Duplicator 1050] Ejecutar programa cargado en la dirección</translation>
    </message>
    <message>
        <location filename="../src/sioworker.cpp" line="275"/>
        <source>[Super Archiver 1050] Set Speed or [Duplicator 1050] Set Load Address</source>
        <translation>[Super Archiver 1050] Establecer velocidad o [Duplicator 1050] Establecer dirección de carga</translation>
    </message>
    <message>
        <location filename="../src/sioworker.cpp" line="277"/>
        <source>[Super Archiver 1050] Read Memory or [Duplicator 1050] Get Sector Data</source>
        <translation>[Super Archiver 1050] Leer memoria o [Duplicator 1050] Obtener datos del sector</translation>
    </message>
    <message>
        <location filename="../src/sioworker.cpp" line="279"/>
        <source>[Super Archiver 1050] Upload and Execute Code or [Duplicator 1050] Upload Data to Drive</source>
        <translation>[Super Archiver 1050] Cargar y ejecutar código o [Duplicator 1050] Cargar datos a Drive</translation>
    </message>
    <message>
        <location filename="../src/sioworker.cpp" line="281"/>
        <source>[Duplicator 1050] Upload Sector Data in Buffer</source>
        <translation>[Duplicator 1050] Cargar datos del sector en buffer</translation>
    </message>
    <message>
        <location filename="../src/sioworker.cpp" line="283"/>
        <source>[Happy 810/1050 Rev. 7] High Speed Write Sector (with verify) or [Duplicator 1050] Write Sector with Deleted Data</source>
        <translation>[Happy 810/1050 Rev. 7] Sector de escritura de alta velocidad (con verificación) o [Duplicator 1050] Sector de escritura con datos eliminados</translation>
    </message>
    <message>
        <location filename="../src/sioworker.cpp" line="285"/>
        <source>[810 Rev. E] Execute Code</source>
        <translation>[810 Rev. E] Código de ejecución</translation>
    </message>
    <message>
        <location filename="../src/sioworker.cpp" line="287"/>
        <location filename="../src/sioworker.cpp" line="382"/>
        <source>Unknown</source>
        <translation>Desconocido</translation>
    </message>
    <message>
        <location filename="../src/sioworker.cpp" line="297"/>
        <source>Run Speed Diagnostic</source>
        <translation>Ejecutar diagnóstico de velocidad</translation>
    </message>
    <message>
        <location filename="../src/sioworker.cpp" line="299"/>
        <source>Run Diagnostic</source>
        <translation>Ejecutar diagnóstico</translation>
    </message>
    <message>
        <location filename="../src/sioworker.cpp" line="301"/>
        <source>Happy Clear Sector Flags (Broadcast)</source>
        <translation>Borrar indicadores de sector Happy (difusión)</translation>
    </message>
    <message>
        <location filename="../src/sioworker.cpp" line="303"/>
        <source>Happy Get Sector Flags (Broadcast)</source>
        <translation>Obtener banderas de sector Happy (difusión)</translation>
    </message>
    <message>
        <location filename="../src/sioworker.cpp" line="305"/>
        <source>Happy Send Sector (Broadcast)</source>
        <translation>Sector de envío Happy (difusión)</translation>
    </message>
    <message>
        <location filename="../src/sioworker.cpp" line="307"/>
        <source>Poll Speed</source>
        <translation>Velocidad de encuesta</translation>
    </message>
    <message>
        <location filename="../src/sioworker.cpp" line="309"/>
        <source>Happy Read Track</source>
        <translation>Leyendo pista Happy</translation>
    </message>
    <message>
        <location filename="../src/sioworker.cpp" line="311"/>
        <source>Super Archiver Write Sector using Index or Happy Read All Sectors</source>
        <translation>Super Archiver escribiendo sector usando en índice o Happy leyendo en todos los sectores</translation>
    </message>
    <message>
        <location filename="../src/sioworker.cpp" line="313"/>
        <source>Super Archiver Read All Sector Statuses or Happy Set Skew Alignment</source>
        <translation>Super Archiver lee todos los estados del sector o alineación de sesgo Happy</translation>
    </message>
    <message>
        <location filename="../src/sioworker.cpp" line="315"/>
        <source>Super Archiver Read Sector using Index or Happy Read Skew Alignment</source>
        <translation>Super Archiver leyendo Sector usando de índice o Happy leyendo alineación sesgada</translation>
    </message>
    <message>
        <location filename="../src/sioworker.cpp" line="317"/>
        <source>Super Archiver Write Track or Happy Write Track</source>
        <translation>Super Archiver escribir Track o Happy escribir Track</translation>
    </message>
    <message>
        <location filename="../src/sioworker.cpp" line="319"/>
        <source>Super Archiver Read Track (128 bytes) or Happy Write All Sectors</source>
        <translation>Super Archiver leer pista (128 bytes) o Happy escritura en todos los sectores</translation>
    </message>
    <message>
        <location filename="../src/sioworker.cpp" line="322"/>
        <source>Happy Set Idle Timeout</source>
        <translation>Establecer tiempo de espera inactivo de Happy</translation>
    </message>
    <message>
        <location filename="../src/sioworker.cpp" line="324"/>
        <source>Happy Set Alternate Device ID</source>
        <translation>Happy Set ID de dispositivo alternativo</translation>
    </message>
    <message>
        <location filename="../src/sioworker.cpp" line="326"/>
        <source>Happy Reinitialize Drive</source>
        <translation>Reinicio de unidad Happy</translation>
    </message>
    <message>
        <location filename="../src/sioworker.cpp" line="328"/>
        <source>Happy Configure Drive</source>
        <translation>Configurar unidad Happy</translation>
    </message>
    <message>
        <location filename="../src/sioworker.cpp" line="330"/>
        <source>Happy Write Track with Skew Alignment</source>
        <translation>Pista de escritura Happy con alineación sesgada</translation>
    </message>
    <message>
        <location filename="../src/sioworker.cpp" line="332"/>
        <source>Happy Init Skew Alignment</source>
        <translation>Happy Init Skew Alignment</translation>
    </message>
    <message>
        <location filename="../src/sioworker.cpp" line="334"/>
        <source>Happy Prepare backup</source>
        <translation>Happy Prepare copia de seguridad</translation>
    </message>
    <message>
        <location filename="../src/sioworker.cpp" line="336"/>
        <source>Super Archiver Set RAM Buffer</source>
        <translation>Super Archiver Set RAM Buffer</translation>
    </message>
    <message>
        <location filename="../src/sioworker.cpp" line="338"/>
        <source>Chip 810 Upload and Execute Code</source>
        <translation>Código de carga y ejecución del chip 810</translation>
    </message>
    <message>
        <location filename="../src/sioworker.cpp" line="340"/>
        <source>Get PERCOM Block</source>
        <translation>Obtener bloque PERCOM</translation>
    </message>
    <message>
        <location filename="../src/sioworker.cpp" line="342"/>
        <source>Set PERCOM Block or Super Archiver Open CHIP</source>
        <translation>Establecer bloque PERCOM o CHIP abierto de Super Archiver</translation>
    </message>
    <message>
        <location filename="../src/sioworker.cpp" line="346"/>
        <location filename="../src/sioworker.cpp" line="354"/>
        <location filename="../src/sioworker.cpp" line="356"/>
        <source>Happy Execute code</source>
        <translation>Código de ejecución Happy</translation>
    </message>
    <message>
        <location filename="../src/sioworker.cpp" line="352"/>
        <source>Super Archiver Get RAM Buffer</source>
        <translation>Super Archiver Obtener buffer RAM</translation>
    </message>
    <message>
        <location filename="../src/sioworker.cpp" line="360"/>
        <source>Super Archiver Set Trace On/Off</source>
        <translation>Super Archiver Establecer seguimiento de encendido/apagado</translation>
    </message>
    <message>
        <location filename="../src/sioworker.cpp" line="362"/>
        <source>Super Archiver Write Fuzzy Sector using Index</source>
        <translation>Super Archiver escribir sector difuso usando índice</translation>
    </message>
    <message>
        <location filename="../src/sioworker.cpp" line="364"/>
        <source>Super Archiver Format with Custom Sector Skewing</source>
        <translation>Formato de Super Archiver con sesgo de sector personalizado</translation>
    </message>
    <message>
        <location filename="../src/sioworker.cpp" line="366"/>
        <source>Super Archiver Read Track (256 bytes)</source>
        <translation>Super Archiver Leer Track (256 bytes)</translation>
    </message>
    <message>
        <location filename="../src/sioworker.cpp" line="368"/>
        <source>Happy High Speed Put Sector (no verify)</source>
        <translation>Happy aplicar el sector Put de alta velocidad (sin verificar)</translation>
    </message>
    <message>
        <location filename="../src/sioworker.cpp" line="370"/>
        <source>Super Archiver Write Fuzzy Sector</source>
        <translation>Super Archiver escribe sector borroso</translation>
    </message>
    <message>
        <location filename="../src/sioworker.cpp" line="372"/>
        <source>Happy High Speed Read Sector</source>
        <translation>Happy sector de lectura de alta velocidad</translation>
    </message>
    <message>
        <location filename="../src/sioworker.cpp" line="374"/>
        <source>Super Archiver Set Speed</source>
        <translation>Super Archiver velocidad fijada</translation>
    </message>
    <message>
        <location filename="../src/sioworker.cpp" line="376"/>
        <source>Super Archiver Read Memory</source>
        <translation>Memoria de lectura de Super Archiver</translation>
    </message>
    <message>
        <location filename="../src/sioworker.cpp" line="378"/>
        <source>Super Archiver Upload and Execute Code</source>
        <translation>Código de carga y ejecución de Super Archiver</translation>
    </message>
    <message>
        <location filename="../src/sioworker.cpp" line="380"/>
        <source>Happy High Speed Write Sector (with verify)</source>
        <translation>Sector de escritura de alta velocidad Happy (con verificación)</translation>
    </message>
    <message>
        <location filename="../src/sioworker.cpp" line="454"/>
        <source>Disk 1 (below autoboot)</source>
        <translation>Disco 1 (con autoboot)</translation>
    </message>
    <message>
        <location filename="../src/sioworker.cpp" line="471"/>
        <source>Disk %1</source>
        <translation>Disco %1</translation>
    </message>
    <message>
        <location filename="../src/sioworker.cpp" line="477"/>
        <source>Printer %1</source>
        <translation>Impresora %1</translation>
    </message>
    <message>
        <location filename="../src/sioworker.cpp" line="480"/>
        <source>Smart device (APE time + URL)</source>
        <translation>Dispositivo inteligente (hora APE + URL)</translation>
    </message>
    <message>
        <location filename="../src/sioworker.cpp" line="483"/>
        <source>RespeQt Client</source>
        <translation>RespeQt Cliente</translation>
    </message>
    <message>
        <location filename="../src/sioworker.cpp" line="489"/>
        <source>RS232 %1</source>
        <translation></translation>
    </message>
    <message>
        <location filename="../src/sioworker.cpp" line="492"/>
        <source>PCLINK</source>
        <translation></translation>
    </message>
    <message>
        <location filename="../src/sioworker.cpp" line="495"/>
        <source>Device $%1</source>
        <translation>Unidad $%1</translation>
    </message>
</context>
<context>
    <name>SmartDevice</name>
    <message>
        <location filename="../src/smartdevice.cpp" line="47"/>
        <source>[%1] Read date/time (%2).</source>
        <translation>[%1] Fecha/hora de lectura (%2).</translation>
    </message>
    <message>
        <location filename="../src/smartdevice.cpp" line="62"/>
        <source>[%1] Read data frame failed</source>
        <translation>[%1] Falló el marco de datos</translation>
    </message>
    <message>
        <location filename="../src/smartdevice.cpp" line="73"/>
        <source>URL [%1] submitted</source>
        <translation>URL [%1] enviada</translation>
    </message>
    <message>
        <location filename="../src/smartdevice.cpp" line="76"/>
        <source>[%1] command: $%2, aux: $%3 NAKed.</source>
        <translation>[%1] comando: $%2, aux: $%3 NAKed.</translation>
    </message>
    <message>
        <location filename="../src/smartdevice.cpp" line="84"/>
        <source>[%1] command: $%2, aux1: $%3, aux2: $%4 NAKed.</source>
        <translation>[%1] commando: $%2, aux1: $%3, aux2: $%4 NAKed.</translation>
    </message>
</context>
<context>
    <name>StandardSerialPortBackend</name>
    <message>
        <location filename="../src/serialport-unix.cpp" line="87"/>
        <location filename="../src/serialport-win32.cpp" line="70"/>
        <location filename="../src/serialport-win32.cpp" line="83"/>
        <source>Cannot open serial port &apos;%1&apos;: %2</source>
        <translation>No se puede abrir puerto serial &apos;%1&apos;: %2</translation>
    </message>
    <message>
        <location filename="../src/serialport-unix.cpp" line="94"/>
        <source>Cannot get serial port status</source>
        <translation>No se puede obtener el estado del puerto serie</translation>
    </message>
    <message>
        <location filename="../src/serialport-unix.cpp" line="99"/>
        <source>Cannot clear RI, RTS and CTS lines in serial port &apos;%1&apos;: %2</source>
        <translation>No se pueden borrar las líneas RI, RTS y CTS en el puerto serie &apos;%1&apos;: %2</translation>
    </message>
    <message>
        <location filename="../src/serialport-unix.cpp" line="131"/>
        <source>Emulation started through standard serial port backend on &apos;%1&apos; with %2 handshaking.</source>
        <translation>Emulación inicia a través de back-end estándar de puerto serial de &apos;%1&apos; con %2 handshaking.</translation>
    </message>
    <message>
        <location filename="../src/serialport-unix.cpp" line="142"/>
        <location filename="../src/serialport-win32.cpp" line="139"/>
        <source>Cannot close serial port: %1</source>
        <translation>No se puede cerrar el puerto serial: %1</translation>
    </message>
    <message>
        <location filename="../src/serialport-unix.cpp" line="245"/>
        <location filename="../src/serialport-unix.cpp" line="258"/>
        <location filename="../src/serialport-unix.cpp" line="288"/>
        <location filename="../src/serialport-win32.cpp" line="260"/>
        <source>Cannot set serial port speed to %1: %2</source>
        <translation>No se puede establecer la velocidad del puerto serial para %1: %2</translation>
    </message>
    <message>
        <location filename="../src/serialport-unix.cpp" line="245"/>
        <source>Closest possible speed is %2.</source>
        <translation>La velocidad posible más cercana es %2.</translation>
    </message>
    <message>
        <location filename="../src/serialport-unix.cpp" line="262"/>
        <location filename="../src/serialport-unix.cpp" line="299"/>
        <location filename="../src/serialport-win32.cpp" line="285"/>
        <source>%1 bits/sec</source>
        <translation>%1 bits/segundo</translation>
    </message>
    <message>
        <location filename="../src/serialport-unix.cpp" line="263"/>
        <location filename="../src/serialport-unix.cpp" line="300"/>
        <location filename="../src/serialport-win32.cpp" line="286"/>
        <source>Serial port speed set to %1.</source>
        <translation>Velocidad del puerto serial configurado para %1.</translation>
    </message>
    <message>
        <location filename="../src/serialport-unix.cpp" line="295"/>
        <source>Failed to set serial port speed to %1</source>
        <translation>Error al establecer la velocidad del puerto serie a %1</translation>
    </message>
    <message>
        <location filename="../src/serialport-unix.cpp" line="315"/>
        <location filename="../src/serialport-unix.cpp" line="432"/>
        <location filename="../src/serialport-win32.cpp" line="302"/>
        <location filename="../src/serialport-win32.cpp" line="419"/>
        <source>Cannot clear serial port read buffer: %1</source>
        <translation>No se puede borrar el buffer de lectura del puerto serie: %1</translation>
    </message>
    <message>
        <location filename="../src/serialport-unix.cpp" line="401"/>
        <location filename="../src/serialport-unix.cpp" line="417"/>
        <location filename="../src/serialport-unix.cpp" line="448"/>
        <source>Cannot retrieve serial port status: %1</source>
        <translation>No se puede recuperar el estado del puerto serie: %1</translation>
    </message>
    <message>
        <location filename="../src/serialport-unix.cpp" line="501"/>
        <location filename="../src/serialport-win32.cpp" line="486"/>
        <source>Data frame checksum error, expected: %1, got: %2. (%3)</source>
        <translation>Los datos de suma de comprobación de errores de trama, que se espera: %1, obtuvo: %2. (%3)</translation>
    </message>
    <message>
        <location filename="../src/serialport-unix.cpp" line="581"/>
        <location filename="../src/serialport-unix.cpp" line="618"/>
        <location filename="../src/serialport-win32.cpp" line="588"/>
        <location filename="../src/serialport-win32.cpp" line="594"/>
        <source>Cannot read from serial port: %1</source>
        <translation>No se puede leer desde el puerto serial: %1</translation>
    </message>
    <message>
        <location filename="../src/serialport-unix.cpp" line="594"/>
        <source>Serial port read timeout. %1 of %2 read in %3 ms</source>
        <translation>Tiempo de espera de lectura de puerto serie. %1 de %2 leído en %3 ms</translation>
    </message>
    <message>
        <location filename="../src/serialport-unix.cpp" line="630"/>
        <source>Serial port write timeout. %1 of %2 written in %3 ms</source>
        <translation>Tiempo de espera de escritura de puerto serie. %1 de %2 escrito en %3 ms</translation>
    </message>
    <message>
        <location filename="../src/serialport-unix.cpp" line="635"/>
        <source>Cannot flush serial port write buffer: %1</source>
        <translation>No se puede vaciar buffer de escritura del puerto serial: %1</translation>
    </message>
    <message>
        <location filename="../src/serialport-win32.cpp" line="87"/>
        <source>Cannot clear DTR line in serial port &apos;%1&apos;: %2</source>
        <translation>No se puede borrar la línea DTR en el puerto serie &apos;%1&apos;: %2</translation>
    </message>
    <message>
        <location filename="../src/serialport-win32.cpp" line="91"/>
        <source>Cannot clear RTS line in serial port &apos;%1&apos;: %2</source>
        <translation>No se puede borrar la línea RTS en el puerto serie &apos;%1&apos;: %2</translation>
    </message>
    <message>
        <location filename="../src/serialport-win32.cpp" line="126"/>
        <source>Emulation started through standard serial port backend on &apos;%1&apos; with %2 handshaking</source>
        <translation>Emulación de iniciado a través de puerto serial estándar backend en &apos;%1&apos; con handshaking de %2</translation>
    </message>
    <message>
        <location filename="../src/serialport-win32.cpp" line="281"/>
        <source>Cannot set serial port timeouts: %1</source>
        <translation>No se puede establecer tiempos de espera del puerto serial: %1</translation>
    </message>
    <message>
        <location filename="../src/serialport-win32.cpp" line="365"/>
        <source>Cannot set serial port event mask: %1</source>
        <translation>No se puede establecer la máscara de evento puerto serial: %1</translation>
    </message>
    <message>
        <location filename="../src/serialport-win32.cpp" line="390"/>
        <location filename="../src/serialport-win32.cpp" line="396"/>
        <source>Cannot waitOnPort for serial port event: %1</source>
        <translation>Cannot waitOnPort for serial port event: %1</translation>
    </message>
    <message>
        <location filename="../src/serialport-win32.cpp" line="580"/>
        <source>Cannot create event: %1</source>
        <translation>No se puede crear el evento: %1</translation>
    </message>
    <message>
        <location filename="../src/serialport-win32.cpp" line="603"/>
        <source>Serial port read timeout.</source>
        <translation>Tiempo de espera de lectura de puerto serie.</translation>
    </message>
    <message>
        <location filename="../src/serialport-win32.cpp" line="618"/>
        <source>Cannot clear serial port write buffer: %1</source>
        <translation>No se puede borrar el buffer de escritura del puerto serial: %1</translation>
    </message>
    <message>
        <location filename="../src/serialport-win32.cpp" line="633"/>
        <location filename="../src/serialport-win32.cpp" line="638"/>
        <source>Cannot write to serial port: %1</source>
        <translation>No se puede escribir en el puerto serial: %1</translation>
    </message>
    <message>
        <location filename="../src/serialport-win32.cpp" line="646"/>
        <source>Serial port write timeout.</source>
        <translation>Tiempo de espera de escritura de puerto serie.</translation>
    </message>
</context>
<context>
    <name>TextPrinterWindow</name>
    <message>
        <location filename="../ui/printers/textprinterwindow.ui" line="14"/>
        <source>RespeQt - Printer text output</source>
        <translation>RespeQt - Salida de texto por impresora</translation>
    </message>
    <message>
        <location filename="../ui/printers/textprinterwindow.ui" line="49"/>
        <location filename="../ui/printers/textprinterwindow.ui" line="52"/>
        <source>Atari Output (Ascii)</source>
        <translation>Salida Atari (Ascii)</translation>
    </message>
    <message>
        <location filename="../ui/printers/textprinterwindow.ui" line="96"/>
        <location filename="../ui/printers/textprinterwindow.ui" line="99"/>
        <source>Atari Output (Atascii)</source>
        <translation>Salida Atari (gráficos)</translation>
    </message>
    <message>
        <location filename="../ui/printers/textprinterwindow.ui" line="123"/>
        <source>toolBar</source>
        <translation>Barra de Herramientas</translation>
    </message>
    <message>
        <location filename="../ui/printers/textprinterwindow.ui" line="159"/>
        <source>Save to a file...</source>
        <translation>Guardar en un archivo...</translation>
    </message>
    <message>
        <location filename="../ui/printers/textprinterwindow.ui" line="162"/>
        <source>Save contents to a file (Ctrl+S)</source>
        <translation>Guardar contenido en un archivo (Ctrl+S)</translation>
    </message>
    <message>
        <location filename="../ui/printers/textprinterwindow.ui" line="165"/>
        <source>Save contents to a file</source>
        <translation>Guardar contenido en un archivo</translation>
    </message>
    <message>
        <location filename="../ui/printers/textprinterwindow.ui" line="168"/>
        <location filename="../ui/printers/textprinterwindow.ui" line="355"/>
        <source>Ctrl+S</source>
        <translation></translation>
    </message>
    <message>
        <location filename="../ui/printers/textprinterwindow.ui" line="177"/>
        <source>Clear</source>
        <translation>Borrar</translation>
    </message>
    <message>
        <location filename="../ui/printers/textprinterwindow.ui" line="180"/>
        <source>Clear contents (Ctrl+C)</source>
        <translation>Borrar el contenido (Ctrl+C)</translation>
    </message>
    <message>
        <location filename="../ui/printers/textprinterwindow.ui" line="183"/>
        <source>Clear contents</source>
        <translation>Borrar el contenido</translation>
    </message>
    <message>
        <location filename="../ui/printers/textprinterwindow.ui" line="186"/>
        <source>Ctrl+C</source>
        <translation></translation>
    </message>
    <message>
        <location filename="../ui/printers/textprinterwindow.ui" line="201"/>
        <source>Word wrap</source>
        <translation>Ajuste de línea</translation>
    </message>
    <message>
        <location filename="../ui/printers/textprinterwindow.ui" line="204"/>
        <source>Toggle word wrapping (Ctrl+W)</source>
        <translation>Activar ajuste de texto (Ctrl+W)</translation>
    </message>
    <message>
        <location filename="../ui/printers/textprinterwindow.ui" line="207"/>
        <source>Toggle word wrapping</source>
        <translation>Activar ajuste de texto</translation>
    </message>
    <message>
        <location filename="../ui/printers/textprinterwindow.ui" line="210"/>
        <source>Ctrl+W</source>
        <translation></translation>
    </message>
    <message>
        <location filename="../ui/printers/textprinterwindow.ui" line="219"/>
        <source>Print</source>
        <translation>Imprimir</translation>
    </message>
    <message>
        <location filename="../ui/printers/textprinterwindow.ui" line="222"/>
        <source>Send contents to printer (Ctrl+P)</source>
        <translation>Enviar contenido a la impresora (Ctrl+P)</translation>
    </message>
    <message>
        <location filename="../ui/printers/textprinterwindow.ui" line="225"/>
        <source>Send contents to printer</source>
        <translation>Enviar contenido a la impresora</translation>
    </message>
    <message>
        <location filename="../ui/printers/textprinterwindow.ui" line="228"/>
        <source>Ctrl+P</source>
        <translation></translation>
    </message>
    <message>
        <location filename="../ui/printers/textprinterwindow.ui" line="238"/>
        <source>Atascii Font</source>
        <translation>Fuente Atascii</translation>
    </message>
    <message>
        <location filename="../ui/printers/textprinterwindow.ui" line="241"/>
        <source>Toggle ATASCII fonts (Alt+F)</source>
        <translation>Alternar las fuentes ATASCII (Alt+F)</translation>
    </message>
    <message>
        <location filename="../ui/printers/textprinterwindow.ui" line="244"/>
        <source>Toggle ATASCII fonts</source>
        <translation>Alternar las fuentes ATASCII</translation>
    </message>
    <message>
        <location filename="../ui/printers/textprinterwindow.ui" line="247"/>
        <source>Alt+F</source>
        <translation></translation>
    </message>
    <message>
        <location filename="../ui/printers/textprinterwindow.ui" line="263"/>
        <source>Font Size</source>
        <translation>Tamaño de la Fuente</translation>
    </message>
    <message>
        <location filename="../ui/printers/textprinterwindow.ui" line="266"/>
        <source>Toggle Font Size (6, 9, 12 pt) (Alt+Shift+F)</source>
        <translation>Cambiar tamaño de la fuente (6, 9, 12 pt) (Alt+Shift+F)</translation>
    </message>
    <message>
        <location filename="../ui/printers/textprinterwindow.ui" line="269"/>
        <source>Toggle Font Size (6, 9, 12 pt)</source>
        <translation>Cambiar tamaño de la fuente (6, 9, 12 pt)</translation>
    </message>
    <message>
        <location filename="../ui/printers/textprinterwindow.ui" line="272"/>
        <source>Alt+Shift+F</source>
        <translation></translation>
    </message>
    <message>
        <location filename="../ui/printers/textprinterwindow.ui" line="291"/>
        <source>Hide/Show Ascii</source>
        <translation>Mostrar/Ocultar Ascii</translation>
    </message>
    <message>
        <location filename="../ui/printers/textprinterwindow.ui" line="294"/>
        <source>Hide/Show Ascii Printer Output (Alt+Shift+H)</source>
        <translation>Ocultar/mostrar la salida por impresora Ascii (Alt+Shift+H)</translation>
    </message>
    <message>
        <location filename="../ui/printers/textprinterwindow.ui" line="297"/>
        <source>Hide/Show Ascii Printer Output</source>
        <translation>Ocultar/mostrar la salida por impresora Ascii</translation>
    </message>
    <message>
        <location filename="../ui/printers/textprinterwindow.ui" line="300"/>
        <source>Alt+Shift+H</source>
        <translation></translation>
    </message>
    <message>
        <location filename="../ui/printers/textprinterwindow.ui" line="322"/>
        <source>HideShow_Atascii</source>
        <translation>Mostrar/Ocultar Ascii</translation>
    </message>
    <message>
        <location filename="../ui/printers/textprinterwindow.ui" line="325"/>
        <source> Hide/Show Atascii Printer Output (Alt+H)</source>
        <translation>Ocultar/Mostrar salida de impresora Atascii (Alt+H)</translation>
    </message>
    <message>
        <location filename="../ui/printers/textprinterwindow.ui" line="328"/>
        <source>Hide/Show Atascii Printer Output</source>
        <translation>Ocultar/Mostrar la salida por impresora Ascii</translation>
    </message>
    <message>
        <location filename="../ui/printers/textprinterwindow.ui" line="331"/>
        <source>Alt+H</source>
        <translation></translation>
    </message>
    <message>
        <location filename="../ui/printers/textprinterwindow.ui" line="346"/>
        <source>Strip Line Numbers</source>
        <translation>Tira números de línea</translation>
    </message>
    <message>
        <location filename="../ui/printers/textprinterwindow.ui" line="349"/>
        <source>Strip Line numbers from the text output (Ctrl-S)</source>
        <translation>Tira números de línea de la salida de texto (Ctrl-S)</translation>
    </message>
    <message>
        <location filename="../ui/printers/textprinterwindow.ui" line="352"/>
        <source>Strip Line numbers from the text output</source>
        <translation>Eliminar números de línea de la salida de texto</translation>
    </message>
</context>
</TS>
