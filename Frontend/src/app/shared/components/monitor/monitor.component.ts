import { CommonModule } from '@angular/common';
import { Component } from '@angular/core';

export interface Message {
  tag: string;
  type: string;
  message: string;
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
      {"tag": "System", "type": "WARNING", "message": "Memory usage is nearing maximum capacity"},
      { "tag": "System", "type": "INFO", "message": "System check complete. All modules active." },
      { "tag": "User", "type": "WARNING", "message": "Password change required." },
      { "tag": "Security", "type": "INFO", "message": "Two-factor authentication enabled." },
      { "tag": "Database", "type": "ERROR", "message": "SQL query failed due to timeout." },
      { "tag": "System", "type": "INFO", "message": "System update applied successfully." },
      { "tag": "Network", "type": "WARNING", "message": "Network congestion detected, retry later." },
      { "tag": "User", "type": "ERROR", "message": "Account locked due to multiple failed login attempts." },
      { "tag": "Database", "type": "INFO", "message": "Data migration process started." },
      { "tag": "Security", "type": "ERROR", "message": "Suspicious login location detected." },
      { "tag": "System", "type": "WARNING", "message": "Service restart scheduled for tonight." },
      { "tag": "User", "type": "INFO", "message": "Profile updated successfully." },
      { "tag": "Network", "type": "ERROR", "message": "Connection timeout while accessing the server." },
      { "tag": "Database", "type": "WARNING", "message": "Slow query detected, optimizing performance." },
      { "tag": "Security", "type": "INFO", "message": "New device authenticated in your account." },
      { "tag": "System", "type": "ERROR", "message": "System crash detected. Rebooting..." },
      { "tag": "User", "type": "INFO", "message": "You have new messages in your inbox." },
      { "tag": "Network", "type": "WARNING", "message": "High packet loss observed on the network." },
      { "tag": "Database", "type": "INFO", "message": "Database maintenance scheduled for tomorrow." },
      { "tag": "Security", "type": "ERROR", "message": "Intrusion detected. Locking down access." },
      { "tag": "System", "type": "WARNING", "message": "Disk space running low. Cleanup required." },
      { "tag": "User", "type": "INFO", "message": "Your subscription has been successfully renewed." },
      { "tag": "Network", "type": "ERROR", "message": "Unable to establish a connection to the proxy." },
      { "tag": "Database", "type": "WARNING", "message": "Potential deadlock detected in the database." },
      { "tag": "Security", "type": "INFO", "message": "Security patch applied successfully." },
      { "tag": "System", "type": "ERROR", "message": "Critical system failure, backup required." },
      { "tag": "User", "type": "INFO", "message": "New update available for your profile." },
      { "tag": "Network", "type": "WARNING", "message": "Network speed below expected levels." },
      { "tag": "Database", "type": "ERROR", "message": "Failed to commit transaction to database." },
      { "tag": "Security", "type": "INFO", "message": "Encryption keys updated successfully." },
      { "tag": "System", "type": "WARNING", "message": "High CPU usage detected." },
      { "tag": "User", "type": "ERROR", "message": "Failed to reset password. Please try again." },
      { "tag": "Network", "type": "INFO", "message": "Network interface reset completed." },
      { "tag": "Database", "type": "WARNING", "message": "Unexpected query delay. Investigating." },
      { "tag": "Security", "type": "ERROR", "message": "Brute force attack detected. Blocking IP." },
      { "tag": "System", "type": "INFO", "message": "Service running optimally after restart." },
      { "tag": "User", "type": "INFO", "message": "You have successfully logged in." },
      { "tag": "Network", "type": "ERROR", "message": "Unable to access the DNS server." },
      { "tag": "Database", "type": "INFO", "message": "Database cluster scaled successfully." },
      { "tag": "Security", "type": "ERROR", "message": "Failed login attempt. Account locked." },
      { "tag": "System", "type": "WARNING", "message": "Scheduled downtime for server maintenance." },
      { "tag": "User", "type": "INFO", "message": "Your account settings have been updated." },
      { "tag": "Network", "type": "WARNING", "message": "Slow internet connection detected." },
      { "tag": "Database", "type": "ERROR", "message": "Database query returned unexpected result." },
      { "tag": "Security", "type": "INFO", "message": "Security audit completed successfully." },
      { "tag": "System", "type": "ERROR", "message": "Unexpected error encountered during system operation." },
      { "tag": "User", "type": "INFO", "message": "You have a new friend request." },
      { "tag": "Network", "type": "ERROR", "message": "Lost connection to the network." },
      { "tag": "Database", "type": "INFO", "message": "Database replication completed successfully." },
      { "tag": "Security", "type": "ERROR", "message": "Access violation detected. Alert sent." }
    
    ];
  
  addMessage(message: Message) {
    this.messages.push(message);
  }

}
