import { Component, OnInit, EventEmitter, Output } from '@angular/core';
import { GamepadService } from '../../../core/services/gamepad-service.service';

@Component({
  selector: 'app-gamepad',
  standalone: true,
  imports: [],
  templateUrl: './gamepad.component.html',
  styleUrl: './gamepad.component.scss',
})
export class GamepadComponent implements OnInit {
  @Output() gamepadConnectionStatus = new EventEmitter<boolean>(); // Emite conexión/desconexión

  connectedGamepad: Gamepad | null = null;
  private previousLeftJoystickState: string = 'Neutral'; // Estado previo para el joystick izquierdo
  private previousRightJoystickState: string = 'Neutral'; // Estado previo para el joystick derecho
  
  constructor(private gamepadService: GamepadService) {}

  ngOnInit(): void {
    // Detectar conexión de joystick
    window.addEventListener('gamepadconnected', (event: GamepadEvent) => {
      this.connectedGamepad = event.gamepad;
      this.gamepadConnectionStatus.emit(true); // Emite true al conectar
      console.log('Gamepad connected:', event.gamepad);
    });

    // Detectar desconexión de joystick
    window.addEventListener('gamepaddisconnected', (event: GamepadEvent) => {
      this.connectedGamepad = null;
      this.gamepadConnectionStatus.emit(false); // Emite false al desconectar
      console.log('Gamepad disconnected:', event.gamepad);
    });

    this.pollJoystick(); // Iniciar polling
  }

  private pollJoystick(): void {
    setInterval(() => {
      const gamepads = this.gamepadService.getConnectedGamepads();
      if (gamepads.length > 0) {
        const gamepad = gamepads[0];
  
        // Crear una copia mutable del array de ejes y botones
        const axes = [...gamepad.axes];
        const buttons = [...gamepad.buttons];
  
        // Obtener direcciones de los joysticks
        const leftJoystickDirection = this.readAnalogs(axes, true);
        const rightJoystickDirection = this.readAnalogs(axes, false);
  
        // Mostrar direcciones en consola
        if (leftJoystickDirection !== 'Neutral') {
          console.log(`Left Joystick: ${leftJoystickDirection}`);
        }
  
        if (rightJoystickDirection !== 'Neutral') {
          console.log(`Right Joystick: ${rightJoystickDirection}`);
        }

        // Leer los botones presionados
        this.readButtons(gamepad); // Leer los botones y mostrar en consola
      }
    }, 100); // Intervalo de 100 ms
  }
  
  readAnalogs(axes: number[], isLeft: boolean): string {
    const tolerance = 0.05; // Tolerancia para ignorar pequeñas desviaciones
  
    // Obtener los ejes de acuerdo con el joystick
    const x = isLeft ? axes[0] : axes[2]; // Eje X del joystick
    const y = isLeft ? axes[1] : axes[5]; // Eje Y del joystick
  
    // Log de valores de los ejes (Depuración)
    const joystickName = isLeft ? 'Left Joystick' : 'Right Joystick';
    // console.log(`${joystickName} - X: ${x.toFixed(2)}, Y: ${y.toFixed(2)}`);
  
    // Determinar el estado actual del joystick
    let currentState = 'Neutral';
  
    if (Math.abs(x) >= tolerance || Math.abs(y) >= tolerance) {
      if (y < -tolerance) {
        currentState = 'Forward'; // Hacia arriba
      } else if (y > tolerance) {
        currentState = 'Backward'; // Hacia abajo
      }
  
      if (x < -tolerance) {
        currentState = 'Left'; // Hacia la izquierda
      } else if (x > tolerance) {
        currentState = 'Right'; // Hacia la derecha
      }
    }
  
    // Imprimir "Brake" solo cuando el joystick se suelta (vuelve a 'Neutral')
    if (currentState === 'Neutral' && (isLeft ? this.previousLeftJoystickState : this.previousRightJoystickState) !== 'Neutral') {
      console.log(`${joystickName} Brake`); // Imprime "Brake"
    }
  
    // Actualizar el estado previo del joystick
    if (isLeft) {
      this.previousLeftJoystickState = currentState;
    } else {
      this.previousRightJoystickState = currentState;
    }
  
    return currentState; // Devuelve el estado actual del joystick
  }

  // Función para detectar todos los botones presionados
  readButtons(gamepad: Gamepad): void {
    gamepad.buttons.forEach((button, index) => {
      if (button.pressed) {
        console.log(`Button ${index} is pressed`);
      }
    });
  }
}
