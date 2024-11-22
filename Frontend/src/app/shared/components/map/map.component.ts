import { Component, OnInit, ElementRef } from '@angular/core';
import * as d3 from 'd3';

@Component({
  selector: 'app-map',
  standalone: true,
  imports: [],
  templateUrl: './map.component.html',
  styleUrl: './map.component.scss',
})
export class MapComponent implements OnInit {
  private width = 500;
  private height = 500;
  private svg: any;
  private maxDistance = 2; // Distancia máxima en metros
  private scaleFactor = this.width / (this.maxDistance * 2); // Escala para convertir metros a pixeles
  private pointsMap: Map<number, any> = new Map();

  ngOnInit() {
    this.createChart();
    this.simulateData(); //FOR TEST ONLY
  }

  createChart() {
    this.svg = d3
      .select('#chart')
      .append('svg')
      .attr('width', this.width)
      .attr('height', this.height)
      .style('background', '#ffffff') // Fondo blanco
      .style('border', '8px solid #213A7D'); // Borde azul oscuro

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

  // Simular recepción de datos
  simulateData() {
    setInterval(() => {
      const distance = Math.random() * this.maxDistance; // Distancia aleatoria entre 0 y 2 metros
      const angle = Math.round(Math.random() * 360); // Ángulo aleatorio entre 0° y 360°

      const { x, y } = this.polarToCartesian(distance, angle);
      this.plotPoint(x, y, angle);
    }, 1000); // Recibir datos cada segundo
  }
  // Convertir distancia y ángulo a coordenadas cartesianas
  polarToCartesian(distance: number, angle: number) {
    const radians = (angle * Math.PI) / 180; // Convertir grados a radianes
    const x = this.width / 2 + distance * this.scaleFactor * Math.cos(radians);
    const y = this.height / 2 - distance * this.scaleFactor * Math.sin(radians); // Invertir eje Y para D3
    return { x, y };
  }
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

  // Función para capturar el mapa como imagen
  captureMap() {
    const svgElement = d3.select('svg').node() as SVGSVGElement;
    const serializer = new XMLSerializer();
    const svgString = serializer.serializeToString(svgElement);
  
    // Crear un canvas
    const canvas = document.createElement('canvas');
    canvas.width = this.width + 10; // Añadir margen extra para incluir bordes
    canvas.height = this.height + 10; // Añadir margen extra para incluir bordes
    const context = canvas.getContext('2d');
  
    // Crear una imagen a partir del SVG
    const image = new Image();
    image.onload = () => {
      // Dibujar la imagen SVG en el canvas
      context?.drawImage(image, 0, 0, this.width + 10, this.height + 10);
      const dataURL = canvas.toDataURL('image/png');
  
      // Descargar la imagen
      const link = document.createElement('a');
      link.href = dataURL;
      link.download = 'map-capture.png';
      link.click();
    };
    image.src = `data:image/svg+xml;base64,${btoa(svgString)}`;
  }
  

}
