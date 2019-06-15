import React, { Component, Fragment } from 'react';
import PropTypes from 'prop-types';
import AccountCircleIcon from '@material-ui/icons/AccountCircle';
import Avatar from '@material-ui/core/Avatar';
import Button from '@material-ui/core/Button';
import CloseIcon from '@material-ui/icons/Close';
import FormControl from '@material-ui/core/FormControl';
import IconButton from '@material-ui/core/IconButton';
import Input from '@material-ui/core/Input';
import InputLabel from '@material-ui/core/InputLabel';
import Paper from '@material-ui/core/Paper';
import Snackbar from '@material-ui/core/Snackbar';
import Typography from '@material-ui/core/Typography';
import withStyles from '@material-ui/core/styles/withStyles';
import AuthService from './AuthService';
import AppBar from '../components/AppBar/AppBar';

const styles = theme => ({
  main: {
    width: 'auto',
    display: 'block', // Fix IE11 issue
    marginLeft: theme.spacing.unit * 3,
    marginRight: theme.spacing.unit * 3,
    [theme.breakpoints.up(400 + theme.spacing.unit * 3 * 2)]: {
      width: 400,
      marginLeft: 'auto',
      marginRight: 'auto',
    },
  },
  paper: {
    marginTop: theme.spacing.unit * 16,
    display: 'flex',
    flexDirection: 'column',
    alignItems: 'center',
    padding: `${theme.spacing.unit * 2}px ${theme.spacing.unit * 3}px ${theme.spacing.unit * 3}px`,
  },
  avatar: {
    margin: theme.spacing.unit,
    backgroundColor: theme.palette.secondary.main,
  },
  form: {
    width: '100%', // IE11 issue
    marginTop: theme.spacing.unit,
  },
  submit: {
    marginTop: theme.spacing.unit * 3,
  },
});

class Register extends Component {
  static propTypes = {
    classes: PropTypes.object.isRequired,
    history: PropTypes.object.isRequired,
  };

  state = {
    errorMsg: '',
    showError: false,
  };

  constructor() {
    super();
    this.handleChange = this.handleChange.bind(this);
    this.handleSubmit = this.handleSubmit.bind(this);
    this.showError = this.showError.bind(this);
    this.dismissError = this.dismissError.bind(this);
    this.auth = new AuthService();
  }

  componentWillMount() {
    const { history } = this.props;
    if (this.auth.loggedIn()) {
      history.replace('/');
    }
  }

  handleChange(e) {
    this.setState({
      [e.target.name]: e.target.value,
    });
  }

  handleSubmit(e) {
    e.preventDefault();
    const { history } = this.props;
    const { username, email, password } = this.state;
    this.auth
      .request('/users', {
        method: 'POST',
        body: JSON.stringify({
          name: username,
          email,
          password,
        }),
      })
      .then(() => {
        history.replace('/login');
      })
      .catch((err) => {
        err.response.json().then((res) => {
          this.showError(res.error);
        });
      });
  }

  showError(msg) {
    this.setState({
      errorMsg: msg,
      showError: true,
    });
  }

  dismissError() {
    this.setState({
      showError: false,
    });
  }

  render() {
    const { classes } = this.props;
    const { errorMsg, showError } = this.state;
    return (
      <Fragment>
        <AppBar />
        <main className={classes.main}>
          <Paper className={classes.paper}>
            <Avatar className={classes.avatar}>
              <AccountCircleIcon />
            </Avatar>
            <Typography component='h1' variant='h5'>
              Sign up
            </Typography>
            <form className={classes.form} onSubmit={this.handleSubmit}>
              <FormControl margin='normal' required fullWidth>
                <InputLabel htmlFor='username'>Username</InputLabel>
                <Input
                  id='username'
                  name='username'
                  autoComplete='username'
                  autoFocus
                  onChange={this.handleChange}
                />
              </FormControl>
              <FormControl margin='normal' required fullWidth>
                <InputLabel htmlFor='email'>Email Address</InputLabel>
                <Input
                  id='email'
                  name='email'
                  autoComplete='email'
                  onChange={this.handleChange}
                />
              </FormControl>
              <FormControl margin='normal' required fullWidth>
                <InputLabel htmlFor='password'>Password</InputLabel>
                <Input
                  id='password'
                  name='password'
                  type='password'
                  autoComplete='current-password'
                  onChange={this.handleChange}
                />
              </FormControl>
              <Button
                type='submit'
                fullWidth
                variant='contained'
                color='primary'
                className={classes.submit}
              >
                Sign up
              </Button>
            </form>
          </Paper>

          <Snackbar
            anchorOrigin={{
              vertical: 'bottom',
              horizontal: 'center',
            }}
            open={showError}
            autoHideDuration={10000}
            onClose={this.dismissError}
            message={<span>{errorMsg}</span>}
            action={(
              <IconButton color='inherit' onClick={this.dismissError}>
                <CloseIcon />
              </IconButton>
            )}
          />
        </main>
      </Fragment>
    );
  }
}

export default withStyles(styles)(Register);
