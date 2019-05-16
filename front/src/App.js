import React, { Component } from "react";
import { Navbar, NavItem, Nav, Grid, Row, Col } from "react-bootstrap";
import Login from "./components/Login"
import Register from "./components/Register"
import CreateNews from "./components/CreateNews"
import Titles from "./components/Titles"
import ArticleFull from "./components/ArticleFull"
import History from "./components/History"
import Delete from "./components/Delete"

const Actions_comment = 0;
const Actions_like = 1;
const Actions_createNews = 2;
const Actions_delUser = 3;

const ROUTES = [
  { index: 0, name: "login" },
  { index: 1, name: "register" },
  { index: 2, name: "create news" },
  { index: 3, name: "last news" },
  { index: 4, name: "history" },
  { index: 5, name: "delete" },

]

export const SecondRoutes = {
  fullArticle: ROUTES.length
};
//export const url = "https://kapust1n.ru"
export const url = "http://localhost:8080"


class App extends Component {
  constructor() {
    super();
    this.state = {
      route: 0
    };
    this.handleClick = this.handleClick.bind(this);
  }

  handleClick(param1, param2) {
    console.log(this.state.route);
    console.log("new rout ", param1);
    this.setState({
      route: param1,
      articleId: param2
    });
  }


  render() {
    console.log('render', this.state.route);
    const route = this.state.route;
    let mainPart;
    const accessRights = localStorage.getItem("accessrights") === null ? 0 : localStorage.getItem("accessrights");
    let localRoutes = ROUTES;
    if (!(accessRights & (1 << Actions_createNews))) {
      const removeIndex = localRoutes.findIndex(elem => elem.name === 'create news')
      if (removeIndex != -1)
        localRoutes.splice(removeIndex, 1)
      console.log(localRoutes)
    }

    if (!(accessRights & (1 << Actions_delUser))) {
      const removeIndex = localRoutes.findIndex(elem => elem.name === 'delete');
      if (removeIndex != -1)
        localRoutes.splice(removeIndex, 1)
    }
    switch (route) {
      case 0:
        mainPart = <Login />
        break;
      case 1:
        mainPart = <Register />
        break;
      case 2:
        mainPart = <CreateNews />
        break;
      case 3:
        mainPart = <Titles handleClick={this.handleClick} />
        break;
      case 4:
        mainPart = <History />
        break;
      case 5:
        mainPart = <Delete />
        break;
      case 6:
        mainPart = <ArticleFull id={this.state.articleId} />
        break;

      default:
        mainPart = <Login />

    }

    return (< div >
      <Navbar>
        <Navbar.Header>
          <Navbar.Brand>
            News Application
        </Navbar.Brand>
        </Navbar.Header>
      </Navbar>
      <Grid>
        <Row>
          <Col md={4} sm={4}>
            <h3>Select action</h3>
            <Nav
              bsStyle="pills"
              stacked
              activeKey={route}
              onSelect={index => {
                this.setState({ route: index });
              }}
            >

              {
                localRoutes.map((rout, index) => (
                  <NavItem key={rout.index} eventKey={rout.index}>{rout.name}</NavItem>
                ))}
            </Nav>
          </Col>
          <Col md={8} sm={8}>
            {mainPart}

          </Col>
        </Row>
      </Grid>
    </div >
    );
  }
}

export default App;