import React from 'react';
import { Redirect, Route, Switch } from 'react-router-dom';
import { ConnectedRouter } from 'connected-react-router';
import PropTypes from 'prop-types';
import Login from './pages/Login/Login';
import FullNavPage from './components/FullNavPage/FullNavPage';
import useAuthRequired from './hooks/useAuthRequired';

const Register = () => <h2>Register</h2>;
const Dashboard = () => <h2>Dashboard</h2>;
const Ledgers = () => <h2>Ledgers</h2>;

const AuthRequiredPages = () => {
  useAuthRequired(true, '/login');
  return (
    <FullNavPage>
      <Switch>
        <Redirect exact from='/' to='/dashboard' />
        <Route path='/dashboard' component={Dashboard} />
        <Route path='/ledgers' component={Ledgers} />
      </Switch>
    </FullNavPage>
  );
};

const App = ({ history }) => (
  <ConnectedRouter history={history}>
    <Switch>
      <Route path='/login' component={Login} />
      <Route path='/register' component={Register} />
      <Route component={AuthRequiredPages} />
    </Switch>
  </ConnectedRouter>
);

App.propTypes = {
  history: PropTypes.object.isRequired,
};

export default App;
