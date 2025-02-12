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
  <summary><i>🌠Características del proyecto</i></summary>
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



<h1 id="authors">✒️ Autores</h1>

* **Guerrico Leonel** [![Repo](https://badgen.net/badge/icon/Eskliar?icon=github&label)](https://github.com/leonelg99)

* **Octavio Perez Balcedo** [![Repo](https://badgen.net/badge/icon/sauersebastian?icon=github&label)](https://github.com/OctavioPB1)
  
*  **Ossola Florecnia** [![Repo](https://badgen.net/badge/icon/LucianoLoyola?icon=github&label)](https://github.com/florencia-ossola)


<h1 id="coordinador">📌 Coordinador</h1>

* **Alan Fabián Castelli** *Profesor - Ayudante* [![Repo](https://badgen.net/badge/icon/aCastelli95?icon=github&label)](https://github.com/aCastelli95)


<!-- Licencia -->
<h1 id="license">📄 Licencia</h1>
<details>
  <summary>Licencia</summary>
  <p>Este proyecto está bajo la Licencia <b>GPL-3.0 license</b>.</p>
  <p>Mira el archivo <code>LICENSE</code> para más detalles.</p>
</details>