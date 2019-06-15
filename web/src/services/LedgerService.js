import ApiService from './ApiService';

export default class LedgerService extends ApiService {
  createLedger(name, currency) {
    return this.fetch(`${this.domain}/ledgers`, {
      method: 'POST',
      body: JSON.stringify({
        name,
        defaultCurrency: currency,
      }),
    });
  }
}
