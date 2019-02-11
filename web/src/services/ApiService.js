export default class ApiService {
  constructor(domain) {
    this.domain = domain || 'http://10.7.7.37:9090'
    this._fetch = this._fetch.bind(this)
  }
    
  _fetch(url, options) {
    const headers = {
      'Accept': 'application/json',
      'Content-Type': 'application/json'
    }
    
    if (this._hasToken()) {
      headers['Authorization'] = 'Bearer ' + this._getToken()
    }
      
    return fetch(url, {
      headers,
      ...options
    }).then(this._checkStatus).then(res => res.json())
  }
    
  _checkStatus(response) {
    if (response.status >= 200 & response.status < 300) {
      return response
    } else {
      var error = new Error(response.statusText)
      error.response = response
      throw error
    }
  }
    
  _getToken() {
    return localStorage.getItem('id_token')
  }
    
  _hasToken() {
    return !!this._getToken()
  }
    
  _setToken(token) {
    localStorage.setItem('id_token', token)
  }
}