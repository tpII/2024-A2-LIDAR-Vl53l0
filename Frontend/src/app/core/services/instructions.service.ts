import { HttpClient } from '@angular/common/http';
import { Injectable } from '@angular/core';
import { catchError, Observable, throwError } from 'rxjs';

@Injectable({
  providedIn: 'root',
})
export class InstructionsService {
  private apiUri = 'http://localhost:8080/instruction';

  constructor(private http: HttpClient) {}

  createInstruction(inst: any): Observable<any> {
    console.log(inst);
    return this.http.post<any>(this.apiUri, inst).pipe(
      catchError((error) => {
        console.error('Error al crear la instrucción:', error);
        // Puedes lanzar un error personalizado o el error original
        return throwError(
          () => new Error('Error al crear la instrucción. Inténtalo de nuevo.')
        );
      })
    );
  }
}
