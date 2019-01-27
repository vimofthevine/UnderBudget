import React, { Component, Fragment } from 'react';
import { Link as RouterLink } from 'react-router-dom';
import AppBar from '@material-ui/core/AppBar';
import Avatar from '@material-ui/core/Avatar';
import Button from '@material-ui/core/Button';
import CloseIcon from '@material-ui/icons/Close';
import FormControl from '@material-ui/core/FormControl';
import IconButton from '@material-ui/core/IconButton';
import Input from '@material-ui/core/Input';
import InputLabel from '@material-ui/core/InputLabel';
import Link from '@material-ui/core/Link';
import LockOutlinedIcon from '@material-ui/icons/LockOutlined';
import Paper from '@material-ui/core/Paper';
import Snackbar from '@material-ui/core/Snackbar';
import Toolbar from '@material-ui/core/Toolbar';
import Typography from '@material-ui/core/Typography';
import withStyles from '@material-ui/core/styles/withStyles';
import AuthService from './AuthService'

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
    marginTop: theme.spacing.unit * 8,
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
  signup: {
    marginTop: theme.spacing.unit * 3,
    width: '100%'
  }
})

class Login extends Component {
    state = {
        errorMsg: "",
        showError: false
    };

    constructor() {
        super();
        this.handleChange = this.handleChange.bind(this);
        this.handleSubmit = this.handleSubmit.bind(this);
        this.showError = this.showError.bind(this);
        this.dismissError = this.dismissError.bind(this);
        this.auth = new AuthService();
    }
    
    handleChange(e) {
        this.setState({
            [e.target.name]: e.target.value
        });
    }
    
    handleSubmit(e) {
        e.preventDefault();
        this.auth.login(this.state.username, this.state.password)
            .then(res => {
                this.props.history.replace('/');
            })
            .catch(err => {
				if (err.response) {
            		err.response.json().then(res => {
                    	this.showError(res.error);
                	})
				}
            })
    }

	showError(msg) {
        this.setState({
            errorMsg: msg,
            showError: true
        });
    }

	dismissError() {
        this.setState({
            showError: false
        });
    }

	componentWillMount() {
        if (this.auth.loggedIn()) {
            this.props.history.replace('/');
        }
    }
    
  render() {
    const  { classes } = this.props;
    return (
      <Fragment>
        <AppBar position="static">
          <Toolbar>
            <Typography color="inherit" variant="h6">
              UnderBudget
            </Typography>
          </Toolbar>
        </AppBar>
        <main className={classes.main}>
          <Paper className={classes.paper}>
            <Avatar className={classes.avatar}>
              <LockOutlinedIcon />
            </Avatar>
            <Typography component="h1" variant="h5">
              Sign in
            </Typography>
            <form className={classes.form}>
              <FormControl margin="normal" required fullWidth>
                <InputLabel htmlFor="username">Username</InputLabel>
                <Input id="username" name="username" autoComplete="username" autoFocus
                    onChange={this.handleChange} />
              </FormControl>
              <FormControl margin="normal" required fullWidth>
                <InputLabel htmlFor="password">Password</InputLabel>
                <Input id="password" name="password" type="password" autoComplete="current-password"
                    onChange={this.handleChange} />
              </FormControl>
              <Button type="submit" fullWidth variant="contained" color="primary" className={classes.submit}
                  onClick={this.handleSubmit}>
                Sign in
              </Button>
            </form>
        
        	<div className={classes.signup}>
        	  <Typography variant="body1" align="right">
        		Don't have an account? <Link color="secondary" component={RouterLink} to="/register">Sign up!</Link>
        	  </Typography>
        	</div>
          </Paper>
        
          <Snackbar
            anchorOrigin={{
              vertical: 'bottom',
              horizontal: 'center'
            }}
            open={this.state.showError}
        	autoHideDuration={10000}
            onClose={this.dismissError}
        	message={<span>{this.state.errorMsg}</span>}
        	action={
        		<IconButton color="inherit" onClick={this.dismissError}>
        			<CloseIcon />
        		</IconButton>
        	}
        	/>
        </main>
      </Fragment>
    );
  }
}

export default withStyles(styles)(Login);