import { Component, Inject, OnInit } from '@angular/core';
import {
  MAT_DIALOG_DATA,
  MatDialogModule,
  MatDialogRef,
} from '@angular/material/dialog';

@Component({
  selector: 'app-dialog-content',
  styleUrls: ['dialog-content.component.scss'],
  template: `
    <h2 mat-dialog-title>Función no implementada!</h2>
    <button mat-button (click)="close()">Cerrar</button>
  `,
})
export class DialogContentComponent {
  constructor(
    public dialogRef: MatDialogRef<DialogContentComponent>,
    @Inject(MAT_DIALOG_DATA) public data: any
  ) {}

  close(): void {
    this.dialogRef.close();
  }
}

import { ElementRef, ViewChild, TemplateRef } from '@angular/core';
import { MatSidenavModule } from '@angular/material/sidenav';
import { MatIconModule } from '@angular/material/icon';
import { AngularSplitModule } from 'angular-split';
import { CommonModule } from '@angular/common';
import { GamepadComponent } from '../../shared/components/gamepad/gamepad.component';
import { MatMenuModule } from '@angular/material/menu';
import { MatDialog } from '@angular/material/dialog';
import { InstructionsService } from '../../core/services/instructions.service';
import { MonitorComponent } from '../../shared/components/monitor/monitor.component';
import { MapComponent } from '../../shared/components/map/map.component';
import { BatteryValueService } from '../../core/services/battery-value.service';
import { inject } from '@angular/core';
import { ModalComponent } from '../../shared/components/modal/modal.component';
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
  imports: [
    MatDialogModule,
    MatSidenavModule,
    MatIconModule,
    AngularSplitModule,
    CommonModule,
    GamepadComponent,
    MatMenuModule,
    MonitorComponent,
    MapComponent,
  ],
  templateUrl: './main.component.html',
  styleUrl: './main.component.scss',
})
export class MainComponent implements OnInit {
  @ViewChild(MapComponent) mapComponent!: MapComponent;
  @ViewChild(MonitorComponent) monitorComponent!: MonitorComponent;

  constructor(
    private InstructionsService: InstructionsService,
    private batteryService: BatteryValueService,
    private dialog: MatDialog
  ) {}

  // Controlar si la sidebar está expandida o no
  isSideNavExpanded = false;
  // Tamaños mínimo y máximo de la sidebar
  minSizeSidenav: number = 7; // Porcentaje para la sidebar colapsada
  maxSizeSidenav: number = 26; // Porcentaje para la sidebar expandida

  // Agregar propiedades para el mando y la batería
  isControllerConnected = true; // Cambiar dinámicamente según el estado real
  batteryLevel = 50; // Cambiar dinámicamente según el nivel de batería

  // Velocidad actual
  speed: string = 'Normal';

  buttons = [
    { icon: 'speed', label: 'Normal' },
    { icon: 'download', label: 'Guardar Mapeo' },
    { icon: 'pause', label: 'PAUSAR' },
    { icon: 'restart_alt', label: 'REINICIAR' },
    { icon: 'cancel', label: 'RESTABLECER' },
  ];

  ngOnInit() {
    setInterval(() => this.getBatteryValue(), 1000);
    this.openWelcomeModal();
  }
  /**
   * Toggles the expansion state of the sidebar.
   * If the sidebar is expanded, it collapses, and vice versa.
   * Logs the current state of the sidebar in the console.
   */
  toggleSidebar(): void {
    this.isSideNavExpanded = !this.isSideNavExpanded;
  }

  /**
   * Dynamically generates the style for a footer figure.
   * Adjusts the width based on whether the sidebar is expanded or collapsed,
   * using predefined minimum and maximum size values.
   * The height remains fixed at 100px.
   *
   * @returns An object containing the dynamic style properties.
   */
  getFooterFigureStyle() {
    return {
      width: `${
        this.isSideNavExpanded ? this.maxSizeSidenav : this.minSizeSidenav
      }%`,
      height: '100px', // Ajusta la altura según el diseño deseado
    };
  }

  /**
   * Handles changes in the gamepad connection status.
   * Updates the `isControllerConnected` variable based on whether the
   * gamepad is connected or disconnected, and logs the status to the console.
   *
   * @param isConnected Indicates if the gamepad is connected (true) or disconnected (false).
   */
  onGamepadStatusChange(isConnected: boolean): void {
    if (isConnected) {
      this.isControllerConnected = false;
    } else {
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

    switch (button.label) {
      case 'PAUSAR':
        button.icon = 'play_arrow';
        button.label = 'REANUDAR';
        this.mapComponent.setup_mapping(false);
        this.monitorComponent.togglePause(false);
        break;
      case 'REANUDAR':
        button.icon = 'pause';
        button.label = 'PAUSAR';
        this.mapComponent.setup_mapping(true);
        this.monitorComponent.togglePause(true);
        break;
      case 'Lento':
        this.openModal();
        /*
        button.label = 'Normal';
        this.speed = 'Normal';
        this.sendInstruction("SpeedUp");*/
        break;
      case 'Normal':
        this.openModal();
        /*
        button.label = 'Rápido';
        this.speed = 'Rápido';
        this.sendInstruction("SpeedUp");*/
        break;
      case 'Rápido':
        this.openModal();
        /*
        button.label = 'Lento';
        this.speed = 'Lento';
        this.sendInstruction("SpeedDown");*/
        break;
      case 'Guardar Mapeo':
        // Funcion para sacar captura al mapa
        this.mapComponent.captureMap();
        break;
      case 'REINICIAR':
        // Funcion para reinicar
        this.resetProcess();
        this.sendInstruction("REBOOT");
        break;
      case 'RESTABLECER':
        // Funcion para restablecer
        this.defaultConfig();
        this.sendInstruction("REBOOT");
        break;
      default:
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
  sendInstruction(inst: string): void {
    //const instruction = { speed: speed };
    const json = {
      instruction: inst,
    };
    this.InstructionsService.createInstruction(json).subscribe({
      next: (response) => {
        console.log('Instrucción creada:', response);
      },
      error: (error) => {
        console.error('Error al crear la instrucción:', error);
      },
    });
  }

  /**
   * Adjusts the speed of the vehicle based on the gamepad input.
   * The speed is changed sequentially between "Lento", "Normal", and "Rápido"
   * when the "SpeedUp" command is received, and the speed is decreased sequentially
   * when the opposite command is received. It also updates the speed button label
   * and sends the corresponding instruction to control the vehicle's speed.
   *
   * @param str The gamepad command, either "SpeedUp" or a command to slow down.
   */
  changeSpeedByGamepad(str: String) {
    const speedButton = this.buttons.find((btn) => btn.label === this.speed);

    if (!speedButton) {
      console.error('Botón de velocidad no encontrado');
      return;
    }

    if (str === 'SpeedUp') {
      this.sendInstruction('SpeedUp');
      //Change speed to next up
      switch (this.speed) {
        case 'Lento':
          speedButton.label = 'Normal';
          this.speed = 'Normal';
          //  this.sendInstruction("MEDIUM");
          break;
        case 'Normal':
          speedButton.label = 'Rápido';
          this.speed = 'Rápido';
          //  this.sendInstruction("FAST");
          break;
        default:
          break;
      }
    } else {
      //Change speed to next down
      this.sendInstruction('SpeedDown');
      switch (this.speed) {
        case 'Rápido':
          speedButton.label = 'Normal';
          this.speed = 'Normal';
          // this.sendInstruction("MEDIUM");
          break;
        case 'Normal':
          speedButton.label = 'Lento';
          this.speed = 'Lento';
          //   this.sendInstruction("SLOW");
          break;
        default:
          break;
      }
    }
  }

  /**
   * Resets the process by restarting the mapping and clearing the messages from the monitor.
   */
  resetProcess() {
    this.mapComponent.restartMapping();
    this.monitorComponent.clearMessages();
  }

  /**
   * Restores the default configuration settings for the vehicle, including setting the speed to "Normal",
   * ensuring that the vehicle is resumed if it was paused, and sending the corresponding instructions.
   * It also resets the mapping and clears the messages on the monitor.
   */
  defaultConfig() {
    const speedButton = this.buttons.find((btn) => btn.label === this.speed);
    const playButton = this.buttons.find((btn) => btn.label === 'REANUDAR');

    if (!speedButton) {
      console.error('Botón de velocidad no encontrado');
      return;
    }

    // velocidad
    console.log('restablecer normal');
    this.speed = 'Normal';
    speedButton.label = 'Normal';

    // si esta en pausa -> reanudar
    if (playButton) {
      playButton.icon = 'pause';
      playButton.label = 'PAUSAR';
      this.mapComponent.setup_mapping(true);
    }

    //this.sendInstruction("MEDIUM");
    this.resetProcess();
    //mandar otras instrucciones para restablecer config
  }

  isMapExpanded: boolean = false;
  isMonitorExpanded: boolean = false;
  /*
  @ViewChild(MapComponent) mapComponent!: MapComponent;
  @ViewChild('monitorComp', { read: ElementRef }) monitorComp!: ElementRef;
  @ViewChild('mapComp', { read: ElementRef }) mapComp!: ElementRef;
  toggleExpand(component: 'map' | 'monitor') {
    if (component === 'map') {
      this.isMapExpanded = !this.isMapExpanded;
      this.isMonitorExpanded = false;
      const state = this.isMapExpanded ? 'expanded' : 'normal';
      this.mapComponent.updateSvgSize(state);
    } else if (component === 'monitor') {
      this.isMonitorExpanded = !this.isMonitorExpanded;
      this.isMapExpanded = false;
      const state = this.isMonitorExpanded ? 'shrunk' : 'normal';
      this.mapComponent.updateSvgSize(state);
    }
      */
  onComponentClick(event: MouseEvent, component: 'map' | 'monitor'): void {
    // Verifica que el clic se originó directamente en el componente
    const target = event.target as HTMLElement;

    if (!target.closest('.console-line') && target.tagName !== 'svg') {
      return;
    }

    if (component === 'map') {
      this.isMapExpanded = !this.isMapExpanded;
      this.isMonitorExpanded = false;
    } else if (component === 'monitor') {
      this.isMonitorExpanded = !this.isMonitorExpanded;
      this.isMapExpanded = false;
    }
  }

  getBatteryValue(): void {
    this.batteryService.getBatteryValue().subscribe({
      next: (data) => {
        if (data !== null) { 
          const level = data.level;
          if (typeof level === 'number') {
            this.batteryLevel = level;
          } else {
            console.warn('Valor de batería inesperado:', level);
          }
        }
      },

      error: (err) => {
        console.error('Error al obtener el nivel de batería:', err);
      },
    });
  }

  openModal(): void {
    this.dialog.open(DialogContentComponent, {
      width: '400px', // Tamaño del modal
      panelClass: 'custom-dialog-container',
    });
  }

  openWelcomeModal(): void {
    this.dialog.open(ModalComponent, {
      width: '720px',      // Ancho predeterminado
      minWidth: '720px',   // Mínimo ancho permitido
      disableClose: true   // Evita que se cierre al hacer clic fuera
    });
  }
  
}
