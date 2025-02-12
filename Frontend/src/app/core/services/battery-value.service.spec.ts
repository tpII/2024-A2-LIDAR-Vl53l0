import { TestBed } from '@angular/core/testing';

import { BatteryValueService } from './battery-value.service';

describe('BatteryValueService', () => {
  let service: BatteryValueService;

  beforeEach(() => {
    TestBed.configureTestingModule({});
    service = TestBed.inject(BatteryValueService);
  });

  it('should be created', () => {
    expect(service).toBeTruthy();
  });
});
