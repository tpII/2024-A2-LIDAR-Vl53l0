import { Injectable } from '@angular/core';

@Injectable({
  providedIn: 'root',
})
export class GamepadService {
  getConnectedGamepads(): Gamepad[] {
    return Array.from(navigator.getGamepads()).filter(
      (gp): gp is Gamepad => gp !== null
    );
  }

 
}
