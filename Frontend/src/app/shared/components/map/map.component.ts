import { Component, OnInit, ElementRef } from '@angular/core';
import * as d3 from 'd3';

@Component({
  selector: 'app-map',
  standalone: true,
  imports: [],
  templateUrl: './map.component.html',
  styleUrl: './map.component.scss'
})
export class MapComponent implements OnInit{
  // 40 x 40
  data = [
    { x: 5, y: 7, obstacle: true },
    { x: 10, y: 12, obstacle: false },
    { x: 15, y: 18, obstacle: true },
    { x: 20, y: 25, obstacle: false },
    { x: 25, y: 30, obstacle: true },
    { x: 30, y: 35, obstacle: false },
    { x: 35, y: 5, obstacle: true },
    { x: 40, y: 20, obstacle: false },
    { x: 8, y: 15, obstacle: true },
    { x: 18, y: 15, obstacle: true },
    { x: 45, y: 10, obstacle: false },
    { x: 43, y: 35, obstacle: true },
    { x: 8, y: 45, obstacle: false },
    { x: 12, y: 32, obstacle: true }
  ];

  ngOnInit() {
    this.createChart();
  }

  createChart() {
    const width = 500;
    const height = 500;

    const svg = d3
      .select('#chart')
      .append('svg')
      .attr('width', width)
      .attr('height', height)
      .style('background', '#f0f0f0');

    // Filtrar solo los obstáculos
    const obstacles = this.data.filter((d) => d.obstacle);

    svg
      .selectAll('circle')
      .data(obstacles)
      .enter()
      .append('circle')
      .attr('cx', (d) => d.x * 10) // Escalar si es necesario
      .attr('cy', (d) => d.y * 10)
      .attr('r', 5) // Tamaño del punto
      .attr('fill', 'red');
  }
}


