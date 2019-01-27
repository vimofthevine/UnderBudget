import decode from 'jwt-decode'

export default class AuthService {
    constructor(domain) {
        this.domain = domain || 'http://10.7.7.37:9090';
        this.fetch = this.fetch.bind(this);
        this.login = this.login.bind(this);
        this.getProfile = this.getProfile.bind(this);
    }
    
    login(username, password) {
        return this.fetch(`${this.domain}/tokens`, {
            method: 'POST',
            body: JSON.stringify({
                'name': username,
                'password': password
            })
        }).then(res => {
            this.setToken(res.token);
            return Promise.resolve(res);
        })
    }
    
    loggedIn() {
        const token = this.getToken();
        return !!token;
    }
    
    setToken(token) {
        localStorage.setItem('id_token', token);
    }
    
    getToken() {
        return localStorage.getItem('id_token');
    }
    
    getProfile() {
        return decode(this.getToken());
    }
    
    logout() {
        const headers = {
            'Authorization': 'Bearer ' + this.getToken(),
        };
        
        const profile = this.getProfile();
       	localStorage.removeItem('id_token');
        
        return fetch(`${this.domain}/tokens/${profile.jti}`, {
            headers,
            method: 'DELETE'
        })
    }
    
    request(url, options) {
        return this.fetch(`${this.domain}${url}`, options);
    }
    
    fetch(url, options) {
        const headers = {
            'Accept': 'application/json',
            'Content-Type': 'application/json'
        };
        
        if (this.loggedIn()) {
            headers['Authorization'] = 'Bearer ' + this.getToken();
        }
        
        return fetch(url, {
            headers,
            ...options
        }).then(this._checkStatus).then(res => res.json());
    }
    
    _checkStatus(response) {
        if (response.status >= 200 && response.status < 300) {
            return response;
        } else {
            var error = new Error(response.statusText);
            error.response = response
            throw error
        }
    }
}