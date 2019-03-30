import React, { Component } from "react";
import { Button, FormGroup, FormControl, ControlLabel, Col } from "react-bootstrap";
import { _parseJSON } from "../../HttpAssist"
import Editor from "../Editor"
import './style.css'

export default class Login extends Component {
    constructor(props) {
        super(props);

        this.state = {
            title: "",
            body: "",
        };
        this.url = "http://localhost:8080/createNews";
        this.handleChangeBody = this.handleChangeBody.bind(this);
        this.handleChangeTitle = this.handleChangeTitle.bind(this);

    }

    validateForm() {
        return this.state.title.length > 0 && this.state.body.length > 0;
    }

    handleChangeTitle(event) {
        this.setState({
            title: event.target.value
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
        }).then(async (res) => {
            if (res.status === 200) {
                return _parseJSON(res);
            } else {
                const response = await _parseJSON(res);
                alert(response["error"]);
                throw new Error();
            }
        }).then(json => {
            alert("News was created!");
            this.setState({
                title: "",
                body: "",
            })
        }).catch((error) => { })
    }
    handleChangeBody(value) {
        this.setState({
            body: value
        });
    }

    /*
      <FormGroup controlId="body" >
                            <ControlLabel>Body</ControlLabel>
                            <FormControl
                                value={this.state.body}
                                onChange={this.handleChange}
                                componentClass="textarea"
                                className="textBody"
                                rows="10"
                            />
                        </FormGroup>
    */
    render() {
        return (
            <Col md={6} sm={6} >

                <div className="CreateNews">
                    <form>
                        <FormGroup controlId="title" >
                            <ControlLabel>Title</ControlLabel>
                            <FormControl
                                autoFocus
                                type="text"
                                value={this.state.title}
                                onChange={this.handleChangeTitle}
                            />
                        </FormGroup>
                        <FormGroup controlId="title" >
                            <ControlLabel>Body</ControlLabel>
                            <Editor setValue={this.handleChangeBody} />
                        </FormGroup>



                        <Button
                            block
                            bsSize="large"
                            disabled={!this.validateForm()}
                            onClick={this.handleSubmit}
                        >
                            Create News
            </Button>
                    </form>
                </div>
            </Col>
        );
    }
}