import React from 'react';
import PropTypes from 'prop-types';
import { makeStyles } from '@material-ui/styles';
import Header from '../Header/Header';

const useStyles = makeStyles(theme => ({
  main: {
    flexGrow: 1,
    padding: theme.spacing(3),
    transition: theme.transitions.create('margin', {
      easing: theme.transitions.easing.sharp,
      duration: theme.transitions.duration.leavingScreen,
    }),
  },
  toolbar: theme.mixins.toolbar,
}));

const FullNavPage = ({ children }) => {
  const classes = useStyles();
  return (
    <div style={{ display: 'flex' }}>
      <Header />
      <main className={classes.main}>
        <div className={classes.toolbar} />
        {children}
      </main>
    </div>
  );
};

FullNavPage.propTypes = {
  children: PropTypes.element.isRequired,
};

export default FullNavPage;
