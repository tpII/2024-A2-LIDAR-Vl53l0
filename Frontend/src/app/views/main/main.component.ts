import { Component } from '@angular/core';
import { MatSidenavModule } from '@angular/material/sidenav';
import { MatIconModule } from '@angular/material/icon';
import { AngularSplitModule } from 'angular-split';
import { CommonModule } from '@angular/common';

@Component({
  selector: 'app-main',
  standalone: true,
  imports: [MatSidenavModule, MatIconModule, AngularSplitModule, CommonModule],
  templateUrl: './main.component.html',
  styleUrl: './main.component.scss',
})
export class MainComponent {
  // Controlar si la sidebar está expandida o no
  isSideNavExpanded = false;
 // Tamaños mínimo y máximo de la sidebar
 minSizeSidenav: number = 7; // Porcentaje para la sidebar colapsada
 maxSizeSidenav: number = 26; // Porcentaje para la sidebar expandida

  // Agregar propiedades para el mando y la batería
  isControllerConnected = true; // Cambiar dinámicamente según el estado real
  batteryLevel = 75; // Cambiar dinámicamente según el nivel de batería

 

  buttons = [
    { icon: 'speed', label: '20m x hora' },
    { icon: 'thermostat', label: 'Máximo 30°' },
    { icon: 'pause', label: 'Pausar' },
    { icon: 'restart_alt', label: 'Reiniciar' },
    { icon: 'cancel', label: 'Restablecer' },
  ];

  // Alternar el estado de la sidebar entre colapsada y expandida
  toggleSidebar(): void {
    this.isSideNavExpanded = !this.isSideNavExpanded;
    console.log('Sidebar expanded:', this.isSideNavExpanded);
  }

  getFooterFigureStyle() {
    return {
      width: `${
        this.isSideNavExpanded ? this.maxSizeSidenav : this.minSizeSidenav
      }%`,
      height: '100px', // Ajusta la altura según el diseño deseado
    };
  }
}
