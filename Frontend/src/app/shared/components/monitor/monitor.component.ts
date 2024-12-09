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

  addMessage(message: Message) {
    if (!this.isPaused) {
      const timestamp = new Date().toLocaleTimeString();
      if(message !== null)
          this.messages.push({ ...message, timestamp });
    }
  }

  ngOnInit(): void {
    this.subscription = interval(1000)
      .pipe(switchMap(() => this.messageService.getLastMessage()))
      .subscribe({
        next: (message) => {
          this.addMessage(message);
          console.log('Lm: ', message);
        },
        error: (err) => {
          console.error('Error al obtener el ultimo mensage', err);
        },
      });
  }
      

  togglePause(pause: boolean): void {
    this.isPaused = pause;
  }

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
