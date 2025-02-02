import { Injectable } from '@angular/core';
import { catchError, Observable, throwError, of } from 'rxjs';
import { HttpClient } from '@angular/common/http';

@Injectable({
  providedIn: 'root'
})
export class BatteryValueService {
  private apiUri = 'http://localhost:8080/battery-level';

  constructor(private http: HttpClient) { }

  getBatteryValue(): Observable<any> {
    return this.http.get<any>(this.apiUri + "/last").pipe(catchError((error) => {
      console.error('Error fetching the last battery value:', error);
      // Devuelve un valor por defecto o un observable vacío
      return of(null); // Puedes devolver un objeto por defecto si lo prefieres
    }));
  }
}
