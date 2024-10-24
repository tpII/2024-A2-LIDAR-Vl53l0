import { Component } from '@angular/core';
import { MatSidenavModule } from '@angular/material/sidenav';
import { MatIconModule } from '@angular/material/icon';
import { AngularSplitModule } from 'angular-split';
import { CommonModule } from '@angular/common';

@Component({
  selector: 'app-sidebar',
  standalone: true,
  imports: [MatSidenavModule, MatIconModule, AngularSplitModule, CommonModule],
  templateUrl: './sidebar.component.html',
  styleUrl: './sidebar.component.scss'
})
export class SidebarComponent {
  // Definir el tamaño inicial de la barra lateral
  isExpanded = false;  // Para controlar si la sidebar está expandida o no

  minSizeSidenav: number = 10;
  maxSizeSidenav: number = 30;

  toggleSidebar(): void {
    this.isExpanded = !this.isExpanded;
  }

}
