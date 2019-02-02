import React from 'react';
import ReactDOM from 'react-dom';
import { HashRouter as Router, Redirect, Route, Switch } from 'react-router-dom';
import CssBaseline from '@material-ui/core/CssBaseline';
import { MuiThemeProvider, createMuiTheme } from '@material-ui/core/styles';
import { blueGrey, green } from '@material-ui/core/colors';
import App from './App';
import Login from './auth/Login';
import Register from './auth/Register';
import * as serviceWorker from './serviceWorker';

const theme = createMuiTheme({
    palette: {
        primary: {
            main: green[600],
        },
        secondary: blueGrey,
    }
})

ReactDOM.render(
  <MuiThemeProvider theme={theme}>
    <Router>
      <div style={{display: 'flex'}}>
        <CssBaseline />
        <Switch>
          <Redirect exact from='/' to='/dashboard' />
          <Route path='/login' component={Login} />
          <Route path='/register' component={Register} />
          <Route component={App} />
        </Switch> 
      </div>
    </Router>
  </MuiThemeProvider>,
  document.getElementById('root'));

// If you want your app to work offline and load faster, you can change
// unregister() to register() below. Note this comes with some pitfalls.
// Learn more about service workers: http://bit.ly/CRA-PWA
serviceWorker.unregister();
