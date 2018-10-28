import React, { Component } from "react";
import { Button, FormGroup, FormControl, ControlLabel, Col } from "react-bootstrap";
import _parseJSON from "../../HttpAssist"
import './style.css'

export default class Login extends Component {
    constructor(props) {
        super(props);

        this.state = {
            title: "",
            body: ""
        };
        this.url = "http://localhost:8080/createNews";
    }

    validateForm() {
        return this.state.title.length > 0 && this.state.body.length > 0;
    }

    handleChange = event => {
        this.setState({
            [event.target.id]: event.target.value
        });
    }

    handleSubmit = event => {
        event.preventDefault();
        const title = this.state.title;
        const body = this.state.body;
        const data = JSON.stringify({
            title: title,
            body: body,
        });

        fetch(this.url, {
            method: 'POST',
            mode: 'cors',
            headers: {
                'Content-Type': 'application/json',
            },
            body: data,
        }).then(res => {
            if (res.status === 200) {
                return _parseJSON(res);
            } else {
                throw new Error();
            }
        }).then(json => { alert("News was created!"); })
            .catch((error) => {
                alert("Cant create news!");
            })
    }

    render() {
        return (
            <Col md={6} sm={6} >
                <div className="CreateNews">
                    <form onSubmit={this.handleSubmit}>
                        <FormGroup controlId="title" >
                            <ControlLabel>Title</ControlLabel>
                            <FormControl
                                autoFocus
                                type="text"
                                value={this.state.title}
                                onChange={this.handleChange}
                            />
                        </FormGroup>
                        <FormGroup controlId="body" >
                            <ControlLabel>Body</ControlLabel>
                            <FormControl
                                value={this.state.body}
                                onChange={this.handleChange}
                                type="textarea"
                                className="textBody"
                            />
                        </FormGroup>
                        <Button
                            block
                            bsSize="large"
                            disabled={!this.validateForm()}
                            type="submit"
                        >
                            Create News
            </Button>
                    </form>
                </div>
            </Col>
        );
    }
}