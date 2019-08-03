// eslint-disable-next-line import/prefer-default-export
export const withAuth = creator => (dispatch, getState) => {
  dispatch(creator({
    headers: {
      Authorization: `Bearer ${getState().auth.token}`,
    },
  }));
};
