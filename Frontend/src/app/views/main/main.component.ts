import { Component } from '@angular/core';
import { MatSidenavModule } from '@angular/material/sidenav';
import { MatIconModule } from '@angular/material/icon';
import { AngularSplitModule } from 'angular-split';
import { CommonModule } from '@angular/common';
import { GamepadComponent } from '../../shared/components/gamepad/gamepad.component';
import { MatMenuModule } from '@angular/material/menu';
import { InstructionsService } from '../../core/services/instructions.service';

/**
 * @component MainComponent
 * @description Main component that integrates other components, such as Sidebar, Map and Monitor. 
 * It also includes buttons for user interaction. When pressed, these buttons trigger specific 
 * actions and send instructions to the backend for processing.
 *
*/

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

  buttons = [
    { icon: 'speed', label: 'Normal' },
    { icon: 'download', label: 'Guardar Mapeo' },
    { icon: 'pause', label: 'PAUSAR' },
    { icon: 'restart_alt', label: 'REINICIAR' },
    { icon: 'cancel', label: 'RESTABLECER' },
  ];

    constructor(private InstructionsService: InstructionsService) {}


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

  /**
   * @description Handles button click events, triggering specific actions based on the button's label.
   * Updates the button's icon and label dynamically, and sends corresponding instructions to the backend 
   * for certain actions such as adjusting speed settings. Prevents event propagation to avoid interference 
   * with other UI elements like the sidenav.
   *
   * @param {Event} event - The event triggered by the button click.
   * @param {any} button - The button object containing its current label and icon.
 */
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
        this.sendInstruction("MEDIUM")
        break;
      case 'Normal':
        button.label = 'Rápido';
        this.sendInstruction("FAST")
        break;
      case 'Rápido':
        button.label = 'Lento';
        this.sendInstruction("SLOW")
        break;
      case 'Guardar Mapeo':
        // Funcion para sacar captura al mapa
        console.log("Guardar mapeo")
        break;
    }
  
  }

  /**
   * @description Sends a speed instruction to the backend using the InstructionsService.
   * Constructs an instruction object with the specified speed and makes a HTTP POST request.
   * Handles the response or error, logging them for debugging purposes.
   *
   * @param {string} speed - The speed value to include in the instruction (e.g., "SLOW", "MEDIUM", "FAST").
 */
  sendInstruction(speed: string): void {
    const instruction = { speed: speed };

    this.InstructionsService.createInstruction(instruction).subscribe({
      next: (response) => {
        console.log('Instrucción creada:', response);
      },
      error: (error) => {
        console.error('Error al crear la instrucción:', error);
      }
    });
  
  }
  
  changeSpeedByGamepad(str: String){
    if(str === "SpeedUp"){
      //Change speed to next up
    } else {
      //Change speed to next down
    }
  }
}
