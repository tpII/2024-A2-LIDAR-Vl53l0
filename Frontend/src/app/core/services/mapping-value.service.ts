import { Injectable } from '@angular/core';
import { HttpClient, HttpErrorResponse } from '@angular/common/http';
import { Observable, catchError, throwError, of } from 'rxjs';

@Injectable({
  providedIn: 'root'
})
export class MappingValueService {

  private apiUri = 'http://localhost:8080/mappingValue';
  constructor(private http: HttpClient) {}

  getMappingValue(): Observable<any>{
    return this.http.get<any>(this.apiUri+"/last").pipe(
      catchError((error) => {
        console.error('Error fetching the last mappingValue:', error);
        // Devuelve un valor por defecto o un observable vacío
        return of(null); // Puedes devolver un objeto por defecto si lo prefieres
      })
    );
  }
}
