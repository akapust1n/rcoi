import * as React from "react";
import ReactMde from "react-mde";
import * as Showdown from "showdown";
import "react-mde/lib/styles/css/react-mde-all.css";

export default class Editor extends React.Component {
    constructor(props) {
        super(props);
        this.state = {
            value: "",
            tab: "write"
        };
        this.converter = new Showdown.Converter({
            tables: true,
            simplifiedAutoLink: true,
            strikethrough: true,
            tasklists: true
        });
        this.setValue = this.props.setValue;

    }

    handleValueChange = (value) => {
        this.setState({ value });
        this.setValue(value);
    };

    handleTabChange = (tab) => {
        this.setState({ tab })
    };

    render() {
        return (
            <div className="container" style={{ width: "600px" }}>
                <ReactMde
                    onChange={this.handleValueChange}
                    value={this.state.value}
                    onTabChange={this.handleTabChange}
                    generateMarkdownPreview={markdown =>
                        Promise.resolve(this.converter.makeHtml(markdown))
                    }
                    selectedTab={this.state.tab}
                />
            </div>
        );
    }
}