import { Component, OnInit, EventEmitter, Output } from '@angular/core';
import { GamepadService } from '../../../core/services/gamepad-service.service';
import { InstructionsService } from '../../../core/services/instructions.service';

@Component({
  selector: 'app-gamepad',
  standalone: true,
  imports: [],
  templateUrl: './gamepad.component.html',
  styleUrl: './gamepad.component.scss',
})
export class GamepadComponent implements OnInit {
  @Output() gamepadConnectionStatus = new EventEmitter<boolean>(); // Emite conexión/desconexión
  @Output() changeValue = new EventEmitter<String>();

  connectedGamepad: Gamepad | null = null;
  private previousLeftJoystickState: string = 'Neutral'; // Estado previo para el joystick izquierdo
  private previousRightJoystickState: string = 'Neutral'; // Estado previo para el joystick derecho
  private previousButtonStates: boolean[] = [];

  constructor(
    private gamepadService: GamepadService,
    private instructionsServide: InstructionsService
  ) {}

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

        // Convertir los arrays readonly a arrays mutables
        const axes = [...gamepad.axes];
        const buttons = [...gamepad.buttons];

        // Obtener direcciones de los joysticks
        this.readAnalogs(axes, true);
        this.readAnalogs(axes, false);

        // Leer los botones presionados
        this.readButtons(buttons); // Leer los botones y mostrar en consola
      }
    }, 100); // Intervalo de 100 ms
  }

  private readAnalogs(axes: number[], isLeft: boolean) {
    const tolerance = 0.05; // Tolerancia para ignorar pequeñas desviaciones
    const joystickName = isLeft ? 'Left Joystick' : 'Right Joystick';

    // Obtener ejes según el joystick
    const x = isLeft ? axes[0] : axes[2];
    const y = isLeft ? axes[1] : axes[5];
    // console.log(`${joystickName} - X: ${x.toFixed(2)}, Y: ${y.toFixed(2)}`);

    // Determinar el estado actual del joystick
    let currentState = 'Neutral';

    // Determinar el estado actual del joystick
    if (Math.abs(x) >= tolerance || Math.abs(y) >= tolerance) {
      if (y < -tolerance) currentState = 'Forward';
      if (y > tolerance) currentState = 'Backward';
      if (x < -tolerance) currentState = 'Left';
      if (x > tolerance) currentState = 'Right';
    }

    // Solo enviar instrucciones cuando el estado cambia
    const previousState = isLeft
      ? this.previousLeftJoystickState
      : this.previousRightJoystickState;
    if (currentState !== previousState) {
      this.sendInstruction(currentState);
      if (isLeft) {
        this.previousLeftJoystickState = currentState;
      } else {
        this.previousRightJoystickState = currentState;
      }
    }
  }

  // Función para detectar todos los botones presionados
  private readButtons(buttons: any[]): void {
    buttons.forEach((button, index) => {
      const wasPressed = this.previousButtonStates[index] || false; // Estado previo (por defecto false)
      const isPressed = button.pressed; // Estado actual

      // Detectar transición de "no presionado" a "presionado"
      if (isPressed && !wasPressed) {
        switch (index) {
          case 0:
            console.log(`Button ${index}: SpeedUp`);
            this.sendInstruction("SpeedUp");
            this.changeValue.emit("SpeedUp");
            break;
          case 2:
            console.log(`Button ${index}: SpeedDown`);
            this.sendInstruction("SpeedDow");
            this.changeValue.emit("SpeedDown");
            break;
          default:
            break;
        }
        console.log(`Button ${index} is pressed`);
      }

      // Actualizar el estado previo
      this.previousButtonStates[index] = isPressed;
    });
  }

  private sendInstruction(inst: any) {
    console.log('INST TO SEND: ' + inst);
    const json = {
      instruction: inst,
    };
    console.log(json);
    this.instructionsServide.createInstruction(json).subscribe(
      (response) => {
        console.log('Success');
      },
      (error) => {
        console.log('Fail');
      }
    );
  }
}
