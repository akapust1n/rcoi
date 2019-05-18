import React, { Component } from "react";
import { Button, FormGroup, FormControl, ControlLabel, Col } from "react-bootstrap";
import { _parseJSON } from "../../HttpAssist"
import './style.css'
import { url } from '../../App'
import VK, { Auth } from "react-vk";

export default class Login extends Component {
    constructor(props) {
        super(props);

        this.state = {
            login: "",
            password: ""
        };
        this.url = `${url}/api/register`;
    }
    validateForm() {
        return this.state.login.length > 0 && this.state.password.length > 0;
    }

    handleChange = event => {
        this.setState({
            [event.target.id]: event.target.value
        });
    }
    vkAuth = (params) => {
        let name = params["first_name"] + params["last_name"]
        console.log(name)
        this.setState({ password: params["uid"].toString(), login: name }, this.handleSubmit)
    }

    handleSubmit = event => {
        if (event != undefined) {
            event.preventDefault();
        }
        const login = this.state.login;
        console.log(login)
        const password = this.state.password;
        const data = JSON.stringify({
            name: login,
            password: password,
        });
        console.log(data)

        fetch(this.url, {
            method: 'POST',
            mode: 'cors',
            headers: {
                'Content-Type': 'application/json',
            },
            body: data,
        }).then(async res => {
            if (res.status === 200) {
                return _parseJSON(res);
            } else {
                const response = await _parseJSON(res);
                alert(response["error"]);
                throw Error();
            }
        }).then(json => { alert("register is ok"); })
            .catch((error) => {
                //  alert("register fails");
            })
    }

    render() {
        return (
            <Col md={4} sm={4} >
                <div className="Register">
                    <form onSubmit={this.handleSubmit}>
                        <FormGroup controlId="login" >
                            <ControlLabel>login</ControlLabel>
                            <FormControl
                                autoFocus
                                type="login"
                                value={this.state.login}
                                onChange={this.handleChange}
                            />
                        </FormGroup>
                        <FormGroup controlId="password" >
                            <ControlLabel>Password</ControlLabel>
                            <FormControl
                                value={this.state.password}
                                onChange={this.handleChange}
                                type="password"
                            />
                        </FormGroup>
                        <Button
                            block
                            bsSize="large"
                            disabled={!this.validateForm()}
                            type="submit"
                        >
                            Register
            </Button>
                        <VK apiId={6988609} >
                            <Auth options={{ "onAuth": this.vkAuth }} />
                        </VK>
                    </form>
                </div>
            </Col>
        );
    }
}