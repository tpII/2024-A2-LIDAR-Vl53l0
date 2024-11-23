import { Component } from '@angular/core';
import { MatSidenavModule } from '@angular/material/sidenav';
import { MatIconModule } from '@angular/material/icon';
import { AngularSplitModule } from 'angular-split';
import { CommonModule } from '@angular/common';
import { GamepadComponent } from '../../shared/components/gamepad/gamepad.component';

@Component({
  selector: 'app-main',
  standalone: true,
  imports: [MatSidenavModule, MatIconModule, AngularSplitModule, CommonModule, GamepadComponent],
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

  onGamepadStatusChange(isConnected: boolean): void {
    if (isConnected) {
      console.log('Gamepad connected!');
    } else {
      console.log('Gamepad disconnected!');
    }
  }
  
}
