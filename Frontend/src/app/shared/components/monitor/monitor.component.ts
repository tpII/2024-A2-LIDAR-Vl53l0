import { CommonModule } from '@angular/common';
import { Component } from '@angular/core';
import { MessagesService } from '../../../core/services/messages.service';
import { Subscription, interval, switchMap } from 'rxjs';

export interface Message {
  tag: string;
  type: 'INFO' | 'WARNING' | 'ERROR';
  message: string;
  timestamp: string;
}

/**
 * @component MonitorComponent
 * @description This component handles the display of monitoring data, such as logs or status updates. 
 * It uses a template and style for the layout and visual presentation. It integrates with other 
 * components for displaying information from sensors or the backend.
 * 
 * The component is self-contained and can be used independently, providing functionality 
 * to show system status, errors, or messages.
 */
@Component({
  selector: 'app-monitor',
  standalone: true,
  imports: [CommonModule],
  templateUrl: './monitor.component.html',
  styleUrl: './monitor.component.scss',
})

export class MonitorComponent {
  isPaused: boolean = false;
  
  messages: any = [];

  private subscription!: Subscription;
  constructor(private messageService: MessagesService) {}

  /**
   * Adds a new message to the messages array if the system is not paused. 
   * The message is augmented with a timestamp of when it was received.
   * 
   * @param message The message to be added to the messages list.
   */
  addMessage(message: Message) {
    if (!this.isPaused) {
      const timestamp = new Date().toLocaleTimeString();
      if(message !== null)
          this.messages.push({ ...message, timestamp });
    }
  }

  /**
   * Initializes the component by subscribing to a service that provides the latest messages.
   * Every second, it fetches the most recent message and adds it to the list of messages.
   * 
   * @note The subscription continues until the component is destroyed or the observable is completed.
   */
  ngOnInit(): void {
    /*this.subscription = interval(1000)
      .pipe(switchMap(() => this.messageService.getLastMessage()))
      .subscribe({
        next: (message) => {
          this.addMessage(message);
          console.log('Lm: ', message);
        },
        error: (err) => {
          console.error('Error al obtener el ultimo mensage', err);
        },
      });*/
  }
    

  /**
   * Toggles the pause state of the message retrieval process.
   * If `pause` is true, the system pauses adding messages; otherwise, it resumes.
   * 
   * @param pause Boolean indicating whether the process should be paused or not.
   */
  togglePause(pause: boolean): void {
    this.isPaused = pause;
  }
  

  /**
   * Clears all the messages stored in the message list.
   * Resets the array to an empty state.
   */
  clearMessages(): void {
    this.messages = []; // Vacía el array de mensajes
  }  
  
  
  toggleExpand() {
    const monitor = document.getElementById('monitor');
    if (monitor) {
      monitor.classList.toggle('expanded');
    }
  }

}
