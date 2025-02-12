import { Component } from '@angular/core';
import { MatDialogRef, MAT_DIALOG_DATA } from '@angular/material/dialog';
import { inject } from '@angular/core';
import { CommonModule } from '@angular/common';
import { MatButtonModule } from '@angular/material/button';
import { MatDialogModule } from '@angular/material/dialog';

/**
 * ModalComponent
 * 
 * Component representing a modal window in the application.
 * It uses Angular Material Dialog to display content in a modal popup.
 */
@Component({
  selector: 'app-modal',
  standalone: true,
  imports: [CommonModule, MatDialogModule, MatButtonModule],
  templateUrl: './modal.component.html',
  styleUrl: './modal.component.scss'
})
export class ModalComponent {
  // Dependency injection for controlling the modal window
  private dialogRef = inject(MatDialogRef<ModalComponent>);

  /**
   * Closes the modal when invoked.
   */
  close(){
    this.dialogRef.close();
  }
}
