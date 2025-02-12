import { Injectable } from '@angular/core';

@Injectable({
  providedIn: 'root',
})

/**
  * Retrieves the list of connected gamepads.
  * It uses the browser's `navigator.getGamepads()` method to access the gamepads.
  * The method filters out any null gamepads and returns an array of active gamepads.
  */
export class GamepadService {
  getConnectedGamepads(): Gamepad[] {
    return Array.from(navigator.getGamepads()).filter(
      (gp): gp is Gamepad => gp !== null
    );
  }

 
}
