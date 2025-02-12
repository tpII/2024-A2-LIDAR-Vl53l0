<!-- Titulo del proyecto -->
# Proyecto - A2-LIDAR-VL53L0X - CYCLOPS

<!-- Logo -->
<div align="center">
  <img src="https://github.com/tpII/2024-A2-LIDAR-Vl53l0/blob/master/Doc/Img/IMG-20250205-WA0008.jpg" width="35%">
</div>

<!-- Descripción del proyecto -->
Originalme **A2-LIDAR-VL53L0X, apodado Cyclops (en honor al héroe de Marvel)**, es un robot vehículo de 3 ruedas, de las cuales dos son motorizadas por motores DC, mientras que la tercera es una rueda loca que permite maniobras ágiles de desplazamiento.

El proyecto se destaca por el uso de un sensor **LiDAR VL53L0X montado sobre un servomotor de giro continuo**, lo que le permite al robot **detectar obstáculos** en su entorno en un radio de **300 grados**. La **MCU ESP32** gestiona todo el funcionamiento, comunicándose con el backend mediante HTTP y MQTT para recibir instrucciones y enviar datos como el nivel de batería, información del sistema y detalles de los obstáculos detectados, respectivamente.

El **backend**, desarrollado en **Maven con SpringBoot junto al broker Mosquitto**, almacena los datos para que puedan ser consultados tanto por el frontend como por el MCU. La base de datos utilizada es **MongoDB**.

El **frontend** está implementado con **Angular 18**, ofreciendo una **interfaz web** que permite **controlar el robot** mediante una serie de botones y con un mando conectado, además de **visualizar información** como el nivel de batería, mensajes de estado y un **mapa interactivo** que muestra los obstáculos detectados como **puntos en coordenadas <distancia, ángulo>.**

El objetivo principal de este proyecto fue desarrollar un **robot vehículo capaz de mapear su entorno y controlarlo** mediante una aplicación web de forma intuitiva y eficiente.

<details>
  <summary><i>:memo: Características del proyecto</i></summary>
  <ol>
    <li><b>Mapeo del Entorno<b></li>
    <p>Se puede decir que es la base del proyecto. Se realiza mediante la combinacion del sensor LiDAR VL53L0X que mide distancia, y un servomotor de giro continuo. Que mediante la logica adeacuada, scanea el entorno, obteniendo el par<distancia,angulo>. Que posteriormente se grafica en un mapa.</p>
    <li>Access Point</li>
    <p>El ESP32 actúa como punto de acceso (AP) para la conexión. De este modo la pc donde se ejectute el backend debe conectarse al la red del ESP32.</p>
    <li>Desarrollo Modular</li>
    <p>El programa está diseñado de manera modular, con componentes separados por funcionalidad, facilitando el mantenimiento y la extensión del proyecto.</p>
    <li>Replicabilidad</li>
    <p>El proyecto está documentado lo suficiente como para permitir replicar el control de un robot similar utilizando el hardware indicado.</p>
    <li>Extensibilidad</li>
    <p>Es posible ampliar el sistema para añadir mejoras de rendimiento o nuevas funcionalidades.</p>
  </ol>
    <li>Base educativa</li>
    <p>El proyecto es ideal para aprender acerca de programación de microcontroladores, conexiones hardware, utilizacion de sensores, integracion de Frontend<->Backend<->Hardware,sistemas concurrentes y de tiempo real, testeo y optimizacion de procesos.</p> </ol>
</details>

<details> 
  <summary><i>:satellite: Tecnologías utilizadas</i></summary>
  <ol> 
    <li>Aplicación web</li>
    <ul> 
      <li>Angular 18: framework frontend utilizado para la construcción de la interfaz de usuario, con un enfoque en componentes independientes y organización modular.</li>
      <li>Angular Material: utilizado para el diseño de la interfaz, proporcionando componentes estilizados y responsivos para una experiencia de usuario consistente.</li>
      <li>HTML, CSS y TypeScript: fundamentales para la estructura, estilo y lógica de la aplicación web, aprovechando las capacidades tipadas de TypeScript para un desarrollo más robusto.</li>
    </ul> 
    <li>Backend</li>
    <ul>
      <li>Java (Spring Boot): framework utilizado para construir el servidor backend, gestionando las solicitudes HTTP y la lógica de negocio.</li>
      <li>Maven: herramienta de gestión de dependencias y construcción del proyecto backend, asegurando un ciclo de desarrollo eficiente.</li>
      <li>MongoDB: base de datos NoSQL utilizada para almacenar la información persistente de la aplicación, con un enfoque en flexibilidad y escalabilidad.</li>
    </ul>
    <li>Comunicación en tiempo real</li>
    <ul>
      <li>Mosquitto Broker (MQTT): utilizado para la comunicación en tiempo real entre la aplicación web y el ESP32, permitiendo la transmisión de datos de forma eficiente y ligera.</li>
    </ul> 
    <li>ESP32</li>
    <ul>
      <li>Modo Soft AP: configurado como punto de acceso para permitir la conexión directa de dispositivos al ESP32.</li>
      <li>Programación en C: se utilizó para implementar la lógica de control del ESP32, incluyendo el manejo de sensores, motores y comunicación MQTT.</li>
      <li>PlatformIO: entorno de desarrollo integrado en Visual Studio Code para la programación y gestión del firmware del ESP32.</li>
      <li>DC Motors: utilizados para el movimiento del dispositivo, controlados mediante PWM para un desplazamiento preciso.</li>
      <li>Servo: utilizado para movimientos angulares específicos, controlado también mediante PWM.</li>
      <li>Sensores I2C: dos sensores conectados al ESP32 mediante el protocolo I2C para la recopilación de datos en tiempo real.</li>
    </ul>
  </ol>
</details>


<!-- Tabla de contenidos -->
<h1 id="table-of-contents">:book: Tabla de contenidos</h1>
<details open="open">
  <summary>Tabla de contenidos</summary>
  <ol>
    <li><a href="#prerequisites-software">➤ Prerequisitos-Software</a></li>
    <li><a href="#installation-esp32">➤ Instalación y Configuración del ESP32</a></li>
    <li><a href="#installation-django-server">➤ Instalación y Configuración del Servidor Django</a></li>
    <li><a href="#execution-steps">➤ Pasos para la Ejecución del Proyecto</a></li>
    <li><a href="#video">➤ Video demostrativo</a></li>
    <li><a href="#bitacora">➤ Bitácora</a></li>
    <li><a href="#authors">➤ Autores</a></li>
    <li><a href="#coordinador">➤ Coordinador</a></li>
  </ol>
</details>

<!-- video explicativo-->
<h1 id="video">:clapper: Video Proyecto </h1>
<p>A continuacion se deja un link, a un video explicativo sobre el robot y su funcionamiento: <a href="https://drive.google.com/file/d/1UwNFf568G4iCPq6gYDLypO1Pdrs1-ocO/view?usp=drive_link">Video</a></p>

<h1 id="bitacora">:bookmark_tabs: Bitácora</h1>

<p>Como parte del proyecto se redacto una bitacora con el "dia a dia" del mismo: <a href="https://github.com/tpII/2024-A4-QLEARNING-ESP32/wiki/Bitacora-A4-%E2%80%90-Crawler-Robot-con-ESP32)">Bitacora</a>.</p>

<h1 id="authors">✒️ Autores</h1>

| Autor                     | GitHub |
|----------------------------|--------|
| **Guerrico Leonel**        | [![Repo](https://badgen.net/badge/icon/leonelg99?icon=github&label)](https://github.com/leonelg99) |
| **Octavio Perez Balcedo**  | [![Repo](https://badgen.net/badge/icon/florencia-ossola?icon=github&label)](https://github.com/OctavioPB1) |
| **Ossola Florencia**       | [![Repo](https://badgen.net/badge/icon/florencia-ossola?icon=github&label)](https://github.com/florencia-ossola) |

---

<h1 id="coordinador">📌 Coordinador</h1>

| Coordinador               | GitHub |
|----------------------------|--------|
| **Alan Fabián Castelli** *Profesor - Ayudante* | [![Repo](https://badgen.net/badge/icon/aCastelli95?icon=github&label)](https://github.com/aCastelli95) |

<!-- Licencia -->
<h1 id="license">📄 Licencia</h1>
<details>
  <summary>Licencia</summary>
  <p>Este proyecto está bajo la Licencia <b>GPL-3.0 license</b>.</p>
  <p>Mira el archivo <code>LICENSE</code> para más detalles.</p>
</details>