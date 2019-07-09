import React, { Fragment } from 'react';
import PropTypes from 'prop-types';
import Avatar from '@material-ui/core/Avatar';
import Paper from '@material-ui/core/Paper';
import Typography from '@material-ui/core/Typography';
import { makeStyles } from '@material-ui/styles';
import SimpleAppBar from '../AppBar/SimpleAppBar';

const useStyles = makeStyles(theme => ({
  main: {
    width: 'auto',
    display: 'block', // Fixes IE11 issue
    marginLeft: theme.spacing(3),
    marginRight: theme.spacing(3),
    [theme.breakpoints.up(400 + theme.spacing(6))]: {
      width: 400,
      marginLeft: 'auto',
      marginRight: 'auto',
    },
  },
  paper: {
    marginTop: theme.spacing(16),
    display: 'flex',
    flexDirection: 'column',
    alignItems: 'center',
    padding: `${theme.spacing(2)}px ${theme.spacing(3)}px ${theme.spacing(
      3,
    )}px`,
  },
  avatar: {
    margin: theme.spacing(1),
    backgroundColor: theme.palette.secondary.main,
  },
  content: {
    width: '100%', // Fixes IE11 issue
    marginTop: theme.spacing(1),
  },
}));

const NoNavPage = ({ avatar, title, children }) => {
  const classes = useStyles();
  return (
    <Fragment>
      <SimpleAppBar />
      <main className={classes.main}>
        <Paper className={classes.paper}>
          <Avatar className={classes.avatar}>{avatar}</Avatar>
          <Typography component='h1' variant='h5'>
            {title}
          </Typography>
          <div className={classes.content}>{children}</div>
        </Paper>
      </main>
    </Fragment>
  );
};

NoNavPage.propTypes = {
  avatar: PropTypes.element.isRequired,
  title: PropTypes.string.isRequired,
  children: PropTypes.element.isRequired,
};

export default NoNavPage;
