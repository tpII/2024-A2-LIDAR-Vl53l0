import { CommonModule } from '@angular/common';
import { Component } from '@angular/core';

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
      { "tag": "System", "type": "INFO", "message": "System initialized successfully." },
      { "tag": "User", "type": "WARNING", "message": "Low battery warning." },
      { "tag": "Network", "type": "ERROR", "message": "Failed to connect to the server." },
      { "tag": "Security", "type": "INFO", "message": "New login detected." },
      { "tag": "Database", "type": "ERROR", "message": "Database connection lost." },
      {"tag": "System", "type": "INFO", "message": "Initialization complete. All systems are running smoothly."},
      {"tag": "Network", "type": "WARNING", "message": "Network latency detected. Performance may be affected."},
      {"tag": "User", "type": "ERROR", "message": "User authentication failed. Please check your credentials."},
      {"tag": "Database", "type": "INFO", "message": "Database backup completed successfully."},
      {"tag": "Security", "type": "ERROR", "message": "Unauthorized access attempt detected. Action required."},
      {"tag": "System", "type": "WARNING", "message": "Memory usage is nearing maximum capacity"}
    ];
    

  /*
  addMessage(message: string) {
    this.messages.push(message);
  }
  */
}
