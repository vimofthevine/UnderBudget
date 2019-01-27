import React, { Component } from 'react';
import logo from './logo.svg';
import AuthService from './auth/AuthService';
import withAuth from './components/withAuth';
import './App.css';

class App extends Component {
    handleLogout() {
        const auth = new AuthService()
        auth.logout()
       	this.props.history.replace('/login')
    }
  render() {
    return (
      <div className="App">
        <header className="App-header">
          <img src={logo} className="App-logo" alt="logo" />
          <p>
            Edit <code>src/App.js</code> and save to reload.
          </p>
          <a
            className="App-link"
            href="https://reactjs.org"
            target="_blank"
            rel="noopener noreferrer"
          >
            Learn React
          </a>
          <p>
        	<button type="button" onClick={this.handleLogout.bind(this)}>Logout</button>
          </p>
        </header>
      </div>
    );
  }
}

export default withAuth(App);