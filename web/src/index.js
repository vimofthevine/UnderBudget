import React from 'react';
import ReactDOM from 'react-dom';
import { HashRouter as Router, Route } from 'react-router-dom';
import CssBaseline from '@material-ui/core/CssBaseline';
import { MuiThemeProvider, createMuiTheme } from '@material-ui/core/styles';
import { lightBlue, green } from '@material-ui/core/colors';
import './index.css';
import App from './App';
import Login from './auth/Login';
import * as serviceWorker from './serviceWorker';

const theme = createMuiTheme({
    palette: {
        primary: {
            main: green[600],
        },
        secondary: lightBlue,
    }
})

ReactDOM.render(
  <MuiThemeProvider theme={theme}>
    <Router>
    	<div>
    		<CssBaseline />
    		<Route exact path='/' component={App} />
    		<Route exact path='/login' component={Login} />
    	</div>
    </Router>
  </MuiThemeProvider>,
  document.getElementById('root'));

// If you want your app to work offline and load faster, you can change
// unregister() to register() below. Note this comes with some pitfalls.
// Learn more about service workers: http://bit.ly/CRA-PWA
serviceWorker.unregister();
