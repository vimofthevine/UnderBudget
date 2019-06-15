export default class ApiService {
  constructor(domain) {
    this.domain = domain || 'http://10.7.7.37:9090';
    this.fetch = this.fetch.bind(this);
  }

  fetch(url, options) {
    const headers = {
      Accept: 'application/json',
      'Content-Type': 'application/json',
    };

    if (this.hasToken()) {
      headers.Authorization = `Bearer ${this.getToken()}`;
    }

    return fetch(url, {
      headers,
      ...options,
    })
      .then(this.checkStatus)
      .then(res => res.json());
  }

  static checkStatus(response) {
    if ((response.status >= 200) && (response.status < 300)) {
      return response;
    }
    const error = new Error(response.statusText);
    error.response = response;
    throw error;
  }

  static getToken() {
    return localStorage.getItem('id_token');
  }

  static hasToken() {
    return !!this.getToken();
  }

  static setToken(token) {
    localStorage.setItem('id_token', token);
  }
}
