<!-- Titulo del proyecto -->
# Proyecto - A2-LIDAR-VL53L0X - CYCLOPS

<!-- Logo -->
<div align="center">
  <img src="https://github.com/tpII/2024-A2-LIDAR-Vl53l0/blob/master/Doc/Img/IMG-20250205-WA0008.jpg" width="35%">;>
</div>

<!-- Descripción del proyecto -->
Originalme **A2-LIDAR-VL53L0X, apodado Cyclops (en honor al héroe de Marvel)**, es un robot vehículo de 3 ruedas, de las cuales dos son motorizadas por motores DC, mientras que la tercera es una rueda loca que permite maniobras ágiles de desplazamiento.

El proyecto se destaca por el uso de un sensor **LiDAR VL53L0X montado sobre un servomotor de giro continuo**, lo que le permite al robot **detectar obstáculos** en su entorno en un radio de **300 grados**. La **MCU ESP32** gestiona todo el funcionamiento, comunicándose con el backend mediante HTTP y MQTT para recibir instrucciones y enviar datos como el nivel de batería, información del sistema y detalles de los obstáculos detectados, respectivamente.

El **backend**, desarrollado en **Maven con SpringBoot junto al broker Mosquitto**, almacena los datos para que puedan ser consultados tanto por el frontend como por el MCU. La base de datos utilizada es **MongoDB**.

El **frontend** está implementado con **Angular 18**, ofreciendo una **interfaz web** que permite **controlar el robot** mediante una serie de botones y con un mando conectado, además de **visualizar información** como el nivel de batería, mensajes de estado y un **mapa interactivo** que muestra los obstáculos detectados como **puntos en coordenadas <distancia, ángulo>.**

El objetivo principal de este proyecto fue desarrollar un **robot vehículo capaz de mapear su entorno y controlarlo** mediante una aplicación web de forma intuitiva y eficiente.