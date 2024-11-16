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
        { "tag": "Security", "type": "ERROR", "message": "Unauthorized access attempt detected. Action required.", "timestamp": "10:14:00" },
        { "tag": "System", "type": "INFO", "message": "System initialized successfully.", "timestamp": "10:14:02" },
        { "tag": "User", "type": "WARNING", "message": "Low battery warning.", "timestamp": "10:14:15" },
        { "tag": "Network", "type": "ERROR", "message": "Failed to connect to the server.", "timestamp": "10:14:30" },
        { "tag": "Security", "type": "INFO", "message": "New login detected.", "timestamp": "10:14:41" },
        { "tag": "Database", "type": "ERROR", "message": "Database connection lost.", "timestamp": "10:14:55" },
        { "tag": "System", "type": "INFO", "message": "Initialization complete. All systems are running smoothly.", "timestamp": "10:15:01" },
        { "tag": "Network", "type": "WARNING", "message": "Network latency detected. Performance may be affected.", "timestamp": "10:15:07" },
        { "tag": "User", "type": "ERROR", "message": "User authentication failed. Please check your credentials.", "timestamp": "10:15:21" },
        { "tag": "Database", "type": "INFO", "message": "Database backup completed successfully.", "timestamp": "10:15:32" },
        { "tag": "Security", "type": "ERROR", "message": "Unauthorized access attempt detected. Action required.", "timestamp": "10:15:39" },
        { "tag": "System", "type": "WARNING", "message": "Memory usage is nearing maximum capacity.", "timestamp": "10:15:45" },
        { "tag": "System", "type": "INFO", "message": "System check complete. All modules active.", "timestamp": "10:17:10" },
        { "tag": "User", "type": "WARNING", "message": "Password change required.", "timestamp": "10:19:00" },
        { "tag": "Security", "type": "INFO", "message": "Two-factor authentication enabled.", "timestamp": "10:20:45" },
        { "tag": "Database", "type": "ERROR", "message": "SQL query failed due to timeout.", "timestamp": "10:22:30" },
        { "tag": "System", "type": "INFO", "message": "System update applied successfully.", "timestamp": "10:25:00" },
        { "tag": "Network", "type": "WARNING", "message": "Network congestion detected, retry later.", "timestamp": "10:27:15" },
        { "tag": "User", "type": "ERROR", "message": "Account locked due to multiple failed login attempts.", "timestamp": "10:28:50" },
        { "tag": "Database", "type": "INFO", "message": "Data migration process started.", "timestamp": "10:30:10" },
        { "tag": "Security", "type": "ERROR", "message": "Suspicious login location detected.", "timestamp": "10:32:00" },
        { "tag": "System", "type": "WARNING", "message": "Service restart scheduled for tonight.", "timestamp": "10:34:00" },
        { "tag": "User", "type": "INFO", "message": "Profile updated successfully.", "timestamp": "10:36:00" },
        { "tag": "Network", "type": "ERROR", "message": "Connection timeout while accessing the server.", "timestamp": "10:38:15" },
        { "tag": "Database", "type": "WARNING", "message": "Slow query detected, optimizing performance.", "timestamp": "10:40:30" },
        { "tag": "Security", "type": "INFO", "message": "New device authenticated in your account.", "timestamp": "10:42:00" },
        { "tag": "System", "type": "ERROR", "message": "System crash detected. Rebooting...", "timestamp": "10:44:30" },
        { "tag": "User", "type": "INFO", "message": "You have new messages in your inbox.", "timestamp": "10:46:15" },
        { "tag": "Network", "type": "WARNING", "message": "High packet loss observed on the network.", "timestamp": "10:48:00" },
        { "tag": "Database", "type": "INFO", "message": "Database maintenance scheduled for tomorrow.", "timestamp": "10:50:00" },
        { "tag": "Security", "type": "ERROR", "message": "Intrusion detected. Locking down access.", "timestamp": "10:52:00" },
        { "tag": "System", "type": "WARNING", "message": "Disk space running low. Cleanup required.", "timestamp": "10:54:00" },
        { "tag": "User", "type": "INFO", "message": "Your subscription has been successfully renewed.", "timestamp": "10:56:30" },
        { "tag": "Network", "type": "ERROR", "message": "Unable to establish a connection to the proxy.", "timestamp": "10:58:00" },
        { "tag": "Database", "type": "WARNING", "message": "Potential deadlock detected in the database.", "timestamp": "11:00:00" },
        { "tag": "Security", "type": "INFO", "message": "Security patch applied successfully.", "timestamp": "11:02:30" },
        { "tag": "System", "type": "ERROR", "message": "Critical system failure, backup required.", "timestamp": "11:04:15" },
        { "tag": "User", "type": "INFO", "message": "New update available for your profile.", "timestamp": "11:06:00" },
        { "tag": "Network", "type": "WARNING", "message": "Network speed below expected levels.", "timestamp": "11:08:00" },
        { "tag": "Database", "type": "ERROR", "message": "Failed to commit transaction to database.", "timestamp": "11:10:00" },
        { "tag": "Security", "type": "INFO", "message": "Encryption keys updated successfully.", "timestamp": "11:12:30" },
        { "tag": "System", "type": "WARNING", "message": "High CPU usage detected.", "timestamp": "11:14:00" },
        { "tag": "User", "type": "ERROR", "message": "Failed to reset password. Please try again.", "timestamp": "11:16:00" },
        { "tag": "Network", "type": "INFO", "message": "Network interface reset completed.", "timestamp": "11:18:00" },
        { "tag": "Database", "type": "WARNING", "message": "Unexpected query delay. Investigating.", "timestamp": "11:20:30" },
        { "tag": "Security", "type": "ERROR", "message": "Brute force attack detected. Blocking IP.", "timestamp": "11:22:00" },
        { "tag": "System", "type": "INFO", "message": "Service running optimally after restart.", "timestamp": "11:24:00" },
        { "tag": "User", "type": "INFO", "message": "You have successfully logged in.", "timestamp": "11:26:00" }
        
    ];
  
  addMessage(message: Message) {
    const timestamp = new Date().toLocaleTimeString(); 
    this.messages.push({ ...message, timestamp });
  }
    

}
