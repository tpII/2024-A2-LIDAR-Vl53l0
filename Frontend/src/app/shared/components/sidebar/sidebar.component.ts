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
  // Controlar si la sidebar está expandida o no
  isExpanded = false;

  // Tamaños mínimo y máximo de la sidebar
  minSizeSidenav: number = 10;  // Porcentaje para la sidebar colapsada
  maxSizeSidenav: number = 30;  // Porcentaje para la sidebar expandida

  // Alternar el estado de la sidebar entre colapsada y expandida
  toggleSidebar(): void {
    this.isExpanded = !this.isExpanded;
  }
}
