import React, { Component } from 'react';
import PropTypes from 'prop-types';
import AuthService from '../auth/AuthService';

export default function withAuth(AuthComponent) {
  const auth = new AuthService();

  return class AuthWrapped extends Component {
    static propTypes = {
      history: PropTypes.object.isRequired,
    }

    constructor() {
      super();
      this.state = {
        user: null,
      };
    }

    componentWillMount() {
      const { history } = this.props;
      if (!auth.loggedIn()) {
        history.replace('/login');
      } else {
        try {
          const profile = auth.getProfile();
          this.setState({
            user: profile,
          });
        } catch (err) {
          auth.logout();
          history.replace('/login');
        }
      }
    }

    render() {
      const { history } = this.props;
      const { user } = this.state;
      if (user) {
        return (
          <AuthComponent history={history} user={user} />
        );
      }
      return null;
    }
  };
}
