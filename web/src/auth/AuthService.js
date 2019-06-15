import decode from 'jwt-decode';
import ApiService from '../services/ApiService';

export default class AuthService extends ApiService {
  constructor(domain) {
    super(domain);
    this.login = this.login.bind(this);
    this.getProfile = this.getProfile.bind(this);
  }

  loggedIn() {
    return this.hasToken();
  }

  getProfile() {
    return decode(this.getToken());
  }

  login(username, password) {
    return this.fetch(`${this.domain}/tokens`, {
      method: 'POST',
      body: JSON.stringify({
        name: username,
        password,
      }),
    }).then((res) => {
      this.setToken(res.token);
      return Promise.resolve(res);
    });
  }

  logout() {
    const headers = {
      Authorization: `Bearer ${this.getToken()}`,
    };

    const profile = this.getProfile();
    localStorage.removeItem('id_token');

    return fetch(`${this.domain}/tokens/${profile.jti}`, {
      headers,
      method: 'DELETE',
    });
  }

  registerUser(name, email, password) {
    return this.fetch(`${this.domain}/users`, {
      method: 'POST',
      body: JSON.stringify({
        name,
        email,
        password,
      }),
    });
  }
}
