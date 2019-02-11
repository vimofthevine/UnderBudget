import decode from 'jwt-decode'
import ApiService from '../services/ApiService'

export default class AuthService extends ApiService {
  constructor(domain) {
    super(domain)
    this.login = this.login.bind(this);
    this.getProfile = this.getProfile.bind(this);
  }
    
  loggedIn() {
    return this._hasToken()
  }
    
  getProfile() {
    return decode(this._getToken());
  }
    
  login(username, password) {
    return this._fetch(`${this.domain}/tokens`, {
      method: 'POST',
      body: JSON.stringify({
        'name': username,
        'password': password
      })
    }).then(res => {
      this._setToken(res.token);
      return Promise.resolve(res);
    })
  }
    
  logout() {
    const headers = {
      'Authorization': 'Bearer ' + this._getToken(),
    };
        
    const profile = this.getProfile();
   	localStorage.removeItem('id_token');
        
    return fetch(`${this.domain}/tokens/${profile.jti}`, {
      headers,
      method: 'DELETE'
    })
  }
    
  registerUser(name, email, password) {
    return this._fetch(`${this.domain}/users`, {
      method: 'POST',
      body: JSON.stringify({
        'name': name,
        'email': email,
        'password': password,
      })
    })
  }
}