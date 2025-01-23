import { Component, OnInit, ElementRef } from '@angular/core';
import { MappingValueService } from '../../../core/services/mapping-value.service';
import * as d3 from 'd3';


export interface Point {
  angle: number;
  distance: number;
}

/**
 * @component MapComponent
 * @description This component is responsible for managing the map view and visualizing the data received from the backend. 
 * It includes functionalities to display and interact with the map and process incoming data based on user inputs.
 * The component also handles the setup and control of the mapping process.
 *
 */
@Component({
  selector: 'app-map',
  standalone: true,
  imports: [],
  templateUrl: './map.component.html',
  styleUrl: './map.component.scss',
})

export class MapComponent implements OnInit {
  private mapping: boolean = true;
  private width = 850;
  private height = 850;
  private svg: any;
  private maxDistance = 2; // Distancia máxima en metros
  private scaleFactor = this.width / (this.maxDistance * 2); // Escala para convertir metros a pixeles
  private pointsMap: Map<number, any> = new Map();

  private pointsToPlot: { distance: number; angle: number }[] = []; // Lista de puntos pendientes
  //private pointsMap = new Map<string, any>(); // Mapa para graficar puntos únicos
  constructor(private mappingValueService: MappingValueService) {}

    /**
   * Initializes the component by creating the chart and plotting points from the backend.
   * This function is triggered when the component is first loaded, and it ensures that the 
   * chart is created and data is fetched from the backend to be visualized.
   */
    ngOnInit() {
      this.createChart();
     // this.plotPointsFromBackend();
     setInterval(() => this.receivePointsFromBackend(), 500); // Llama cada 500ms
     setInterval(() => this.plotStoredPoints(), 100); // Graficar cada 100ms
     // this.simulateData(); //FOR TEST ONLY
    }

  receivePointsFromBackend(): void {
    this.mappingValueService.getMappingValue().subscribe((data) => {
      if (data && Array.isArray(data)) {
        // Almacena los nuevos puntos en la lista
        this.pointsToPlot.push(...data);
      } else {
        console.warn('Datos inesperados recibidos del backend:', data);
      }
    });
  }

  plotStoredPoints(): void {
    if (!this.pointsToPlot.length || !this.mapping) {
      return; // Nada que graficar
    }

    this.pointsToPlot.forEach((point) => {
      // Convertir de polar (ángulo, distancia) a coordenadas cartesianas (x, y)
      const { x, y } = this.polarToCartesian(point.distance, point.angle);

      // Crear una clave única para identificar el punto
    //  const key = `${point.angle}-${point.distance}`;

     // if (this.pointsMap.has(key)) {
      if (this.pointsMap.has(point.angle)) {
        // Si el punto ya existe, actualiza su posición
        const existingPoint = this.pointsMap.get(point.angle);
        existingPoint
          .attr('cx', x)
          .attr('cy', y);
      } else {
        // Si no existe, crea un nuevo punto en el mapa
        const newPoint = this.svg
          .append('circle')
          .attr('cx', x)
          .attr('cy', y)
          .attr('r', 4)
          .attr('fill', 'blue');
       //   this.pointsMap.set(key, newPoint);
          this.pointsMap.set(point.angle, newPoint);
      }
    });

    // Vaciar la lista de puntos ya procesados
    this.pointsToPlot = [];
  }

  /**
 * Sets the mapping process state.
  * This function updates the `mapping` variable, controlling whether the mapping process is active or paused.
  * 
  * @param mapping_process - A boolean indicating whether to start or stop the mapping process.
  */
  setup_mapping(mapping_process: boolean){
    this.mapping = mapping_process;
  }

  /**
   * Creates and initializes the chart with SVG elements.
   * It sets the dimensions of the chart, adds a white background, 
   * draws a border around the content area, and places the robot in the center of the chart.
   */
  createChart() {
    this.svg = d3
      .select('#chart')
      .append('svg')
      .attr('width', this.width)
      .attr('height', this.height)
      .style('background', '#ffffff') // Fondo blanco

      // Crear borde alrededor del contenido
      this.svg
        .append('rect')
        .attr('x', 0)
        .attr('y', 0)
        .attr('width', this.width)
        .attr('height', this.height)
        .attr('fill', 'none')
        .attr('stroke', '#213A7D')
        .attr('stroke-width', 16);
    // Filtrar solo los obstáculos

    // Dibujar el robot en el centro
    this.svg
      .append('rect')
      .attr('x', this.width / 2 - 10)
      .attr('y', this.height / 2 - 10)
      .attr('width', 20)
      .attr('height', 20)
      .attr('fill', '#213A7D');

  }

  /**
   * Receives data from the backend and plots the points on the map.
   * Converts polar coordinates (distance, angle) into Cartesian coordinates 
   * and either updates existing points or creates new ones to display on the map.
   */
  plotPointsFromBackend(): void {
    this.mappingValueService.getMappingValue().subscribe((data) => {

      if (!this.mapping) { //revisar
        return; 
      }

      data.forEach((point: { distance: number; angle: number }) => {
        // Convertir de polar a cartesiano
        const { x, y } = this.polarToCartesian(point.distance, point.angle);
  
        // Graficar el punto en el mapa
        if (this.pointsMap.has(point.angle)) {
          // Actualizar el punto existente
          const existingPoint = this.pointsMap.get(point.angle);
          existingPoint
            .attr('cx', x)
            .attr('cy', y);
        } else {
          // Crear un nuevo punto
          const newPoint = this.svg
            .append('circle')
            .attr('cx', x)
            .attr('cy', y)
            .attr('r', 4)
            .attr('fill', 'blue');
          this.pointsMap.set(point.angle, newPoint);
        }
      });
    });
  }
  
  /**
   * Clears all stored points on the map by removing their corresponding graphical elements 
   * from the SVG and resetting the points map.
   */
  restartMapping(){
    // Recorrer todos los puntos almacenados en el mapa
    this.pointsMap.forEach((point) => {
      point.remove(); // Eliminar el elemento gráfico del SVG
    });

    // Limpiar el mapa de puntos
    this.pointsMap.clear();
  }
  

  // Simular recepción de datos
  simulateData() {
    setInterval(() => {
      if (!this.mapping) {
        return; 
      }

      const distance = Math.random() * this.maxDistance; // Distancia aleatoria entre 0 y 2 metros
      const angle = Math.round(Math.random() * 360); // Ángulo aleatorio entre 0° y 360°

      const { x, y } = this.polarToCartesian(distance, angle);
      this.plotPoint(x, y, angle);
    }, 1000); // Recibir datos cada segundo
  }

  /**
   * Converts polar coordinates (distance and angle) to Cartesian coordinates (x, y).
   * The angle is first converted to radians and used to calculate the x and y values based 
   * on the distance and scale factor. The y-axis is inverted to fit the D3 coordinate system.
   * 
   * @param distance The distance from the origin (in some unit).
   * @param angle The angle in degrees.
   * @returns The Cartesian coordinates (x, y).
   */
  polarToCartesian(distance: number, angle: number) {
    const radians = (angle * Math.PI) / 180; // Convertir grados a radianes
    const x = this.width / 2 + distance * this.scaleFactor * Math.cos(radians);
    const y = this.height / 2 - distance * this.scaleFactor * Math.sin(radians); // Invertir eje Y para D3
    return { x, y };
  }

  /**
   * Plots a point on the SVG based on the given x, y coordinates. If a point already exists 
   * for the given angle, it updates the position of the existing point. Otherwise, it creates 
   * a new point at the specified coordinates and adds it to the points map.
   * 
   * @param x The x-coordinate to place the point.
   * @param y The y-coordinate to place the point.
   * @param angle The angle of the point for identification in the map.
   */
  plotPoint(x: number, y: number, angle: number) {
    // Verificar si ya existe un punto para el ángulo dado
    if (this.pointsMap.has(angle)) {
      // Actualizar posición del punto existente
      const existingPoint = this.pointsMap.get(angle);
      existingPoint
        .attr('cx', x)
        .attr('cy', y)
        .attr('fill','black'); 
    } else {
      // Crear un nuevo punto y agregarlo al mapa
      const newPoint = this.svg
        .append('circle')
        .attr('cx', x)
        .attr('cy', y)
        .attr('r', 4)
        .attr('fill', 'black');
      this.pointsMap.set(angle, newPoint);
    }
  }

  /**
   * Captures the current SVG map as an image, including a border around it. 
   * The image is saved as a PNG file with the border added around the map for clarity.
   */
  captureMap() {
    const svgElement = this.svg.node() as SVGSVGElement;
    const serializer = new XMLSerializer();
    const svgString = serializer.serializeToString(svgElement);
  
    // Dimensiones con borde incluido
    const borderSize = 8; // Tamaño del borde
    const canvas = document.createElement('canvas');
    canvas.width = this.width + 2 * borderSize;
    canvas.height = this.height + 2 * borderSize;
    const context = canvas.getContext('2d');
  
    // Dibujar el borde uniformemente
    context!.fillStyle = '#213A7D'; // Color del borde
    context!.fillRect(0, 0, canvas.width, canvas.height);
  
    const image = new Image();
    image.onload = () => {
      // Dibujar la imagen SVG centrada dentro del borde
      context?.drawImage(image, borderSize, borderSize, this.width, this.height);
      const dataURL = canvas.toDataURL('image/png');
  
      // Descargar la imagen
      const link = document.createElement('a');
      link.href = dataURL;
      link.download = 'map-capture-with-borders.png';
      link.click();
    };
    image.src = `data:image/svg+xml;base64,${btoa(svgString)}`;
  }
  
  

  toggleExpand() {
    const chart = document.getElementById('chart');
    if (chart) {
      chart.classList.toggle('expanded');
    }
  }

  

  updateSvgSize(state: 'normal' | 'expanded' | 'shrunk') {
    const svgElement = d3.select('#chart svg');
    if (svgElement) {
      if (state === 'expanded') {
        svgElement.attr('width', 800).attr('height', 600);
      } else if (state === 'shrunk') {
        svgElement.attr('width', 850).attr('height', 850);
      } else {
        svgElement.attr('width', 950).attr('height', 850);
      }
    }
  }
  
  

}
