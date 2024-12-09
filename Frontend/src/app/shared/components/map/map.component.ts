import { Component, OnInit, ElementRef } from '@angular/core';
import { MappingValueService } from '../../../core/services/mapping-value.service';
import * as d3 from 'd3';

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

  constructor(private mappingValueService: MappingValueService) {}

  ngOnInit() {
    this.createChart();
    //this.plotPointsFromBackend();
    this.simulateData(); //FOR TEST ONLY
  }

  setup_mapping(mapping_process: boolean){
    this.mapping = mapping_process;
  }

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

  // Recibir datos del back y graficarlos
  plotPointsFromBackend(): void {
    this.mappingValueService.getMappingValue().subscribe((data) => {
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
  
  

  // Simular recepción de datos
  simulateData() {
    setInterval(() => {
      if (!this.mapping) {
        return; // Si el mapeo está en pausa, no grafiques puntos
      }
      console.log(this.mapping);
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
