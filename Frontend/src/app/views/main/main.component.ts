import { Component } from '@angular/core';
import { MatSidenavModule } from '@angular/material/sidenav';
import { MatIconModule } from '@angular/material/icon';
import { AngularSplitModule } from 'angular-split';
import { CommonModule } from '@angular/common';
import { GamepadComponent } from '../../shared/components/gamepad/gamepad.component';
import { MatMenuModule } from '@angular/material/menu';

@Component({
  selector: 'app-main',
  standalone: true,
  imports: [MatSidenavModule, MatIconModule, AngularSplitModule, CommonModule, GamepadComponent, MatMenuModule],
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

 //speedButton = { icon: 'speed', label: '20m x hora' }
 //speeds = ["Lento", "Normal", "Rápido"]; // Opciones de velocidad

  buttons = [
    { icon: 'speed', label: 'Normal' },
    { icon: 'thermostat', label: 'Máximo 30°' },
    { icon: 'pause', label: 'PAUSAR' },
    { icon: 'restart_alt', label: 'REINICIAR' },
    { icon: 'cancel', label: 'RESTABLECER' },
  ];


    toggleSpeedMenu(event: MouseEvent) {
      event.stopPropagation();  // Esto previene que el clic se propague al sidebar
      // Aquí va tu lógica para mostrar el menú de velocidad
    }
  

  onSpeedClick(speed: string) {
    console.log('Velocidad seleccionada:', speed);
  }

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
  
    switch (button.label) {
      case 'PAUSAR':
        button.icon = 'play_arrow';
        button.label = 'REANUDAR';
        break;
      case 'REANUDAR':
        button.icon = 'pause';
        button.label = 'PAUSAR';
        break;
      case 'Lento':
        button.label = 'Normal';
        break;
      case 'Normal':
        button.label = 'Rápido';
        break;
      case 'Rápido':
        button.label = 'Lento';
        break;
      // Añadir más casos para otros botones si es necesario
    }
  
  }
  
  changeSpeedByGamepad(str: String){
    if(str === "SpeedUp"){
      //Change speed to next up
    } else {
      //Change speed to next down
    }
  }
}
