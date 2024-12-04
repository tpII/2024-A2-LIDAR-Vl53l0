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

  // Agregar propiedades para el mando y la batería
  isControllerConnected = true; // Cambiar dinámicamente según el estado real
  batteryLevel = 75; // Cambiar dinámicamente según el nivel de batería

 

  buttons = [
    { icon: 'speed', label: '20m x hora' },
    { icon: 'thermostat', label: 'Máximo 30°' },
    { icon: 'pause', label: 'PAUSAR' },
    { icon: 'restart_alt', label: 'REINICIAR' },
    { icon: 'cancel', label: 'RESTABLECER' },
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

  onGamepadStatusChange(isConnected: boolean): void {
    if (isConnected) {
      console.log('Gamepad connected!');
      this.isControllerConnected = false;
    } else {
      console.log('Gamepad disconnected!');
      this.isControllerConnected = true;
    }
  }

  onButtonClick(event: Event, button: any): void {
    // Detiene la propagación del evento para evitar que afecte al sidenav
    event.stopPropagation();
    console.log('Botón presionado:', button.label);
  }
  
  changeSpeedByGamepad(str: String){
    if(str === "SpeedUp"){
      //Change speed to next up
    } else {
      //Change speed to next down
    }
  }
}
