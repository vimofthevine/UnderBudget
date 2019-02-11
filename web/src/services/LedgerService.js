import ApiService from './ApiService'

export default class LedgerService extends ApiService {
  createLedger(name, currency) {
    return this._fetch(`${this.domain}/ledgers`, {
      method: 'POST',
      body: JSON.stringify({
        'name': name,
        'defaultCurrency': currency
      })
    })
  }
}