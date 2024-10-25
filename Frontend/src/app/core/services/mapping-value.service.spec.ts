import { TestBed } from '@angular/core/testing';

import { MappingValueService } from './mapping-value.service';

describe('MappingValueService', () => {
  let service: MappingValueService;

  beforeEach(() => {
    TestBed.configureTestingModule({});
    service = TestBed.inject(MappingValueService);
  });

  it('should be created', () => {
    expect(service).toBeTruthy();
  });
});
