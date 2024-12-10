import { HttpClient, HttpErrorResponse } from '@angular/common/http';
import { Injectable } from '@angular/core';
import { Observable, catchError, throwError, of } from 'rxjs';

@Injectable({
  providedIn: 'root',
})
export class MessagesService {
  private apiUri = 'http://localhost:8080/message';
  constructor(private http: HttpClient) {}

  getLastMessage(): Observable<any> {
    return this.http.get<any>(this.apiUri+"/last").pipe(
      catchError((error) => {
        console.error('Error fetching the last message:', error);
        // Devuelve un valor por defecto o un observable vacío
        return of(null); // Puedes devolver un objeto por defecto si lo prefieres
      })
    );
  }
}
