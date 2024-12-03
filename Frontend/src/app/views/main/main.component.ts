import { Component } from '@angular/core';
import { MatSidenavModule } from '@angular/material/sidenav';
import { MatIconModule } from '@angular/material/icon';
import { AngularSplitModule } from 'angular-split';
import { CommonModule } from '@angular/common';
import { MonitorComponent } from "../../shared/components/monitor/monitor.component";
import { MapComponent } from "../../shared/components/map/map.component";

@Component({
  selector: 'app-main',
  standalone: true,
  imports: [MatSidenavModule, MatIconModule, AngularSplitModule, CommonModule, MonitorComponent, MapComponent],
  templateUrl: './main.component.html',
  styleUrl: './main.component.scss',
})
export class MainComponent {
  // Controlar si la sidebar está expandida o no
  isSideNavExpanded = false;

  // Tamaños mínimo y máximo de la sidebar
  minSizeSidenav: number = 7; // Porcentaje para la sidebar colapsada
  maxSizeSidenav: number = 26; // Porcentaje para la sidebar expandida

  // Alternar el estado de la sidebar entre colapsada y expandida
  toggleSidebar(): void {
    this.isSideNavExpanded = !this.isSideNavExpanded;
    console.log('Sidebar expanded:', this.isSideNavExpanded);
  }

  getFooterFigureStyle() {
    return {
      width: `${this.isSideNavExpanded ? this.maxSizeSidenav : this.minSizeSidenav}%`,
      height: '100px', // Ajusta la altura según el diseño deseado
    };
  }
/*
  toggleExpand(component: 'map' | 'monitor') {
    const map = document.getElementById('map');
    const monitor = document.getElementById('monitor');
  
    if (map && monitor) {
      if (component === 'map') {
        map.classList.toggle('expanded');
        monitor.classList.remove('expanded');
      } else if (component === 'monitor') {
        monitor.classList.toggle('expanded');
        map.classList.remove('expanded');
      }
    }
  }
*/

  isMapExpanded: boolean = false;
  isMonitorExpanded: boolean = false;

  toggleExpand(component: 'map' | 'monitor') {
    if (component === 'map') {
      if (this.isMapExpanded) {
        // Volver a tamaño normal
        this.isMapExpanded = false;
        this.isMonitorExpanded = false;
      } else {
        // Expandir mapa y ocultar monitor
        this.isMapExpanded = true;
        this.isMonitorExpanded = false;
      }
    } else if (component === 'monitor') {
      if (this.isMonitorExpanded) {
        // Volver a tamaño normal
        this.isMonitorExpanded = false;
        this.isMapExpanded = false;
      } else {
        // Expandir monitor y reducir mapa
        this.isMonitorExpanded = true;
        this.isMapExpanded = false;
      }
    }

  }
 
  
}
