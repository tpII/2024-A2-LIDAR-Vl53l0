import { CommonModule } from '@angular/common';
import { Component } from '@angular/core';

export interface Message {
  tag: string;
  type:  'INFO' | 'WARNING' | 'ERROR';
  message: string;
  timestamp: string;
}

@Component({
  selector: 'app-monitor',
  standalone: true,
  imports: [CommonModule],
  templateUrl: './monitor.component.html',
  styleUrl: './monitor.component.scss'
})
export class MonitorComponent {
  messages = 
    [
        { "tag": "System", "type": "INFO", "message": "System initialized successfully.", "timestamp": "10:00:00" },
        { "tag": "User", "type": "WARNING", "message": "Low battery warning.", "timestamp": "10:02:00" },
        { "tag": "Network", "type": "ERROR", "message": "Failed to connect to the server.", "timestamp": "10:03:30" },
        { "tag": "Security", "type": "INFO", "message": "New login detected.", "timestamp": "10:05:00" },
        { "tag": "Database", "type": "ERROR", "message": "Database connection lost.", "timestamp": "10:06:15" },
        { "tag": "System", "type": "INFO", "message": "Initialization complete. All systems are running smoothly.", "timestamp": "10:08:00" },
        { "tag": "Network", "type": "WARNING", "message": "Network latency detected. Performance may be affected.", "timestamp": "10:09:45" },
        { "tag": "User", "type": "ERROR", "message": "User authentication failed. Please check your credentials.", "timestamp": "10:11:00" },
        { "tag": "Database", "type": "INFO", "message": "Database backup completed successfully.", "timestamp": "10:12:30" },
        { "tag": "Security", "type": "ERROR", "message": "Unauthorized access attempt detected. Action required.", "timestamp": "10:14:00" }
      
    ];
  
  addMessage(message: Message) {
    const timestamp = new Date().toLocaleTimeString(); 
    this.messages.push({ ...message, timestamp });
  }
    

}
