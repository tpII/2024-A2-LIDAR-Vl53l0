import { Component, OnInit, EventEmitter, Output } from '@angular/core';
import { GamepadService } from '../../../core/services/gamepad-service.service';
import { InstructionsService } from '../../../core/services/instructions.service';

/**
 * GamepadComponent
 * 
 * Component responsible for managing the gamepad input and emitting changes related
 * to the gamepad connection status and values for further processing.
 */
@Component({
  selector: 'app-gamepad',
  standalone: true,
  imports: [],
  templateUrl: './gamepad.component.html',
  styleUrl: './gamepad.component.scss',
})
export class GamepadComponent implements OnInit {
  // Event emitter to notify about the gamepad connection status
  @Output() gamepadConnectionStatus = new EventEmitter<boolean>(); // Emite conexión/desconexión

  // Event emitter to notify of any changes in the gamepad values
  @Output() changeValue = new EventEmitter<String>();

  // Holds the current connected gamepad object, or null if no gamepad is connected
  connectedGamepad: Gamepad | null = null;
  // Store the previous state of the left joystick, initialized to 'Neutral'
  private previousLeftJoystickState: string = 'Neutral'; 
  // Store the previous state of the right joystick, initialized to 'Neutral'
  private previousRightJoystickState: string = 'Neutral'; 
  // Array to store the previous states of buttons on the gamepad
  private previousButtonStates: boolean[] = [];

   /**
   * Constructor for initializing the GamepadComponent with the necessary services.
   * 
   * @param gamepadService - Service to manage gamepad-related logic.
   * @param instructionsServide - Service to handle instructions related to the gamepad.
   */
  constructor(
    private gamepadService: GamepadService,
    private instructionsServide: InstructionsService
  ) {}

  /**
   * Initializes the component by setting up event listeners for gamepad connection and disconnection.
   * It emits the connection status when a gamepad is connected or disconnected and starts polling
   * the joystick for updates. The polling allows for continuous monitoring of the joystick input.
   */
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

  /**
   * Continuously polls the connected gamepad at a fixed interval (100 ms).
   * It retrieves the current gamepad state, including joystick movements and button presses,
   * and processes them by passing the joystick axes and button data to the appropriate methods.
   * This ensures that the joystick and button states are regularly checked and updated.
   */
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

  /**
   * Processes the input from the left or right joystick, determining the joystick's direction
   * based on its x and y axes values. It compares the current state with the previous state to
   * detect changes, and if the state has changed, it sends the appropriate instruction (e.g., 
   * 'Forward', 'Backward', 'Left', 'Right', 'Brake'). It also applies a tolerance to ignore
   * small deviations in the joystick's position.
   */
  private readAnalogs(axes: number[], isLeft: boolean) {
    const tolerance = 0.05; // Tolerancia para ignorar pequeñas desviaciones
    const joystickName = isLeft ? 'Left Joystick' : 'Right Joystick';

    // Obtener ejes según el joystick
    const x = isLeft ? axes[0] : axes[2];
    const y = isLeft ? axes[1] : axes[5];
    // console.log(`${joystickName} - X: ${x.toFixed(2)}, Y: ${y.toFixed(2)}`);

    // Determinar el estado actual del joystick
    let currentState = 'Brake';

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

  /**
   * Monitors the state of each button on the gamepad. It detects when a button is pressed and executes
   * the corresponding action (e.g., sending specific instructions like 'SpeedUp', 'SpeedDown', 'Brake',
   * 'ABORT', 'REBOOT'). It also updates the state of each button to track changes between polling intervals.
   */
  private readButtons(buttons: any[]): void {
    buttons.forEach((button, index) => {
      const wasPressed = this.previousButtonStates[index] || false; // Estado previo (por defecto false)
      const isPressed = button.pressed; // Estado actual

      // Detectar transición de "no presionado" a "presionado"
      if (isPressed && !wasPressed) {
        switch (index) {
          case 0:
            console.log(`Button ${index}: SpeedUp`);
            //this.sendInstruction('SpeedUp');
            //this.changeValue.emit('SpeedUp');
            break;
          case 2:
            console.log(`Button ${index}: SpeedDown`);
            //this.sendInstruction('SpeedDown');
            //this.changeValue.emit('SpeedDown');
            break;
          case 7:
            console.log(`Button ${index}: Brake`);
            this.sendInstruction('Brake');
            break;
          case 8:
            console.log(`Button ${index}: ABORT`);
            this.sendInstruction('ABORT');
            break;
          case 9:
            console.log(`Button ${index}: REBOOT`);
            this.sendInstruction('REBOOT');
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

  /**
   * Sends an instruction to the backend by calling the `createInstruction` method of the `InstructionsService`.
   * The instruction is logged to the console before being sent. A success or failure message is logged based 
   * on the response from the backend.
   */
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
